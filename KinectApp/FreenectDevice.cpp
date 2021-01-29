#include "FreenectDevice.hpp"

FreenectDevice::FreenectDevice(freenect_context *context, int index) : Freenect::FreenectDevice(context, index), m_VideoBuffer(videoResolution * 3), m_DepthBuffer(depthResolution), m_NewRGBFrame(false), m_NewDepthFrame(false) { }

FreenectDevice::~FreenectDevice() { }

void FreenectDevice::VideoCallback(void *rgbRaw, uint32_t timestamp) {
	// Thread safety
	m_RGBMutex.lock();
	// Copy to buffer
	uint8_t *rgb = static_cast<uint8_t*>(rgbRaw);
	copy(rgb, rgb + getVideoBufferSize(), m_VideoBuffer.begin());
	// New frame available
	m_NewRGBFrame = true;
	m_RGBMutex.unlock();
}

void FreenectDevice::DepthCallback(void *depthRaw, uint32_t timestamp) {
	// Thread safety
	m_DepthMutex.lock();
	// Copy to buffer
	uint16_t *depth = static_cast<uint16_t*>(depthRaw);
	copy(depth, depth + getDepthBufferSize(), m_DepthBuffer.begin());
	// New frame available
	m_NewDepthFrame = true;
	m_DepthMutex.unlock();
}

bool FreenectDevice::getRGB(std::vector<uint8_t> &buffer) {
	// Thread safety
	m_RGBMutex.lock();
	if (m_NewRGBFrame) {
		// Swap with new RGB data
		buffer.swap(m_VideoBuffer);
		// Frame has been retrieved
		m_NewRGBFrame = false;
		m_RGBMutex.unlock();
		return true;
	} else {
		m_RGBMutex.unlock();
		return false;
	}
}

bool FreenectDevice::getDepth(std::vector<uint16_t> &buffer) {
	// Thread safety
	m_DepthMutex.lock();
	if (m_NewDepthFrame) {
		// Swap with new depth data
		buffer.swap(m_DepthBuffer);
		// Frame has been retrieved
		m_NewDepthFrame = false;
		m_DepthMutex.unlock();
		return true;
	} else {
		m_DepthMutex.unlock();
		return false;
	}
}
