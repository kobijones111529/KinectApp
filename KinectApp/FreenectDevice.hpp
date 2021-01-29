#pragma once

#include <vector>

#include "libfreenect.hpp"

#include "Mutex.hpp"

class FreenectDevice : public Freenect::FreenectDevice {
public:
	const static size_t videoResolution = 640 * 480;
	const static size_t depthResolution = 640 * 480;
	
	FreenectDevice(freenect_context *context, int index);
	~FreenectDevice();
	
	void VideoCallback(void *rgbRaw, uint32_t timestamp);
	void DepthCallback(void *depthRaw, uint32_t timestamp);
	bool getRGB(std::vector<uint8_t> &buffer);
	bool getDepth(std::vector<uint16_t> &buffer);
	
private:
	std::vector<uint8_t> m_VideoBuffer;
	std::vector<uint16_t> m_DepthBuffer;
	Mutex m_RGBMutex;
	Mutex m_DepthMutex;
	bool m_NewRGBFrame;
	bool m_NewDepthFrame;
};
