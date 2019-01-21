/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"

#include "OgreRoot.h"
#include "OgreLogManager.h"
#include "OgreImage.h"
#include "OgreException.h"


#include "OgreAviSaver.h"
#include "AVIGenerator.h"



namespace Ogre {

	AviSaver::AviSaver() : 
mAviGen(), 
mRenderWindow(0), 
mRenderWindowPixelBox(0), 
mRate(30), 
mTimer(0), 
mLastFrameTime(0),
mTempBufferForSave(0)
	{

	}
	//---------------------------------------------------------------------
	AviSaver::~AviSaver()
	{
	}
	//---------------------------------------------------------------------
	/** @copydoc Plugin::getName */
	const String& AviSaver::getName() const
	{
		static String name("AviSaver");
		return name;
	}
	//---------------------------------------------------------------------
	/** @copydoc Plugin::install */
	void AviSaver::install()
	{
	}
	//---------------------------------------------------------------------
	/** @copydoc Plugin::initialise */
	void AviSaver::initialise()
	{
		mAviGen  = new CAVIGenerator();
		
		mRenderWindow = Root::getSingleton().getAutoCreatedWindow();
		mTimer = Root::getSingleton().getTimer();


		mRenderWindowPixelBox = new PixelBox (
			mRenderWindow->getWidth(),
			mRenderWindow->getHeight(),
			1,
			PF_R8G8B8
			);

		// alloc
		mRenderWindowPixelBox->data = new BYTE[mRenderWindowPixelBox->getConsecutiveSize()];
		mTempBufferForSave = new BYTE[mRenderWindowPixelBox->getConsecutiveSize()];

		// set rate
		mAviGen->SetRate(mRate);

		// give info about bitmap
		BITMAPINFOHEADER bitmapHeader;
		ZeroMemory(&bitmapHeader, sizeof(BITMAPINFOHEADER));

		bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitmapHeader.biWidth = mRenderWindowPixelBox->getWidth();
		bitmapHeader.biHeight  = mRenderWindowPixelBox->getHeight();
		bitmapHeader.biPlanes = 1;
		bitmapHeader.biBitCount = PixelUtil::getNumElemBits(mRenderWindowPixelBox->format);
		bitmapHeader.biCompression = BI_RGB;
		bitmapHeader.biSizeImage = mRenderWindowPixelBox->getConsecutiveSize();
		bitmapHeader.biXPelsPerMeter = 0;
		bitmapHeader.biYPelsPerMeter = 0;
		bitmapHeader.biClrUsed = 0;
		bitmapHeader.biClrImportant = 0;
		mAviGen->SetBitmapHeader(&bitmapHeader);

		// set filename, extension ".avi" is appended if necessary
		mAviGen->SetFileName("test.avi");

		HRESULT hr = mAviGen->InitEngine();

		if (FAILED(hr))
		{

			LogManager::getSingleton().logMessage(
				String("AviSaver::AviSaver mAviGen->InitEngine: ") + mAviGen->GetLastErrorMessage());
			return;

		}

		Root::getSingleton().addFrameListener(this);
	}
	//---------------------------------------------------------------------
	/** @copydoc Plugin::shutdown */
	void AviSaver::shutdown()
	{
		Root::getSingleton().removeFrameListener(this);

		// releasing engine and memory
		mAviGen->ReleaseEngine();
		delete[] mRenderWindowPixelBox->data;
		delete[] mTempBufferForSave;

		delete mAviGen;
	}
	//---------------------------------------------------------------------
	/** @copydoc Plugin::uninstall */
	void AviSaver::uninstall()
	{
	}
	//---------------------------------------------------------------------
	/** @copydoc FrameListener::frameEnded */
	bool AviSaver::frameEnded(const FrameEvent& evt)
	{
 
		unsigned long  milliseconds = mTimer->getMilliseconds(); 
		if (milliseconds > mLastFrameTime + mRate)
		{
			mLastFrameTime = milliseconds;
			mRenderWindow->copyContentsToMemory(*mRenderWindowPixelBox, RenderTarget::FB_BACK);

			size_t biBitCount = PixelUtil::getNumElemBits(mRenderWindowPixelBox->format);
			size_t width = mRenderWindowPixelBox->getWidth();
			size_t height = mRenderWindowPixelBox->getHeight();
			size_t sizeImage = mRenderWindowPixelBox->getConsecutiveSize();
			size_t rowLengthInBytes = biBitCount * width / 8;

			// flip the lines
			BYTE * data = static_cast<BYTE *>(mRenderWindowPixelBox->data);
			for(int i = 0 ; i < height - 1 ; i++ )
			{
				memcpy(&mTempBufferForSave[rowLengthInBytes * (height - i - 1)], &data[rowLengthInBytes * i], rowLengthInBytes);

			}

			mRenderWindowPixelBox->data;

			HRESULT hr = mAviGen->AddFrame(mTempBufferForSave);
			if (FAILED(hr))
			{
					LogManager::getSingleton().logMessage(
						String("AviSaver::postRenderTargetUpdate: ") + mAviGen->GetLastErrorMessage());
				
			}

	
		}

		return true;

	
	}

}
