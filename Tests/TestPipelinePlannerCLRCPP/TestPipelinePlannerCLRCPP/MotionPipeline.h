// RBotFirmware
// Rob Dobson 2016

#pragma once

#include "MotionRingBuffer.h"
#include "MotionPipelineElem.h"
#include <vector>

class MotionPipeline
{
private:
	MotionRingBufferPosn _pipelinePosn;
	std::vector<MotionPipelineElem> _pipeline;

public:
	MotionPipeline() :
		_pipelinePosn(0)
	{
	}

	void init(int pipelineSize)
	{
		_pipeline.resize(pipelineSize);
		_pipelinePosn.init(pipelineSize);
	}

	// Clear the pipeline
	void clear()
	{
		_pipelinePosn.clear();
	}

	unsigned int count()
	{
		return _pipelinePosn.count();
	}

	// Check if ready to accept data
	bool canAccept()
	{
		return _pipelinePosn.canPut();
	}

	// Add to pipeline
	bool add(MotionPipelineElem& elem)
	{
		// Check if full
		if (!_pipelinePosn.canPut())
			return false;

		// Add the item
		_pipeline[_pipelinePosn._putPos] = elem;
		_pipelinePosn.hasPut();
		return true;
	}

	// Add to pipeline
	bool add(AxisFloats& pt1, AxisFloats& pt2)
	{
		// Check if full
		if (!_pipelinePosn.canPut())
			return false;

		// Add the item
		MotionPipelineElem elem(pt1, pt2);
		_pipeline[_pipelinePosn._putPos] = elem;
		_pipelinePosn.hasPut();
		return true;
	}

	// Can get from queue (i.e. not empty)
	bool canGet()
	{
		return _pipelinePosn.canGet();
	}

	// Get from queue
	bool get(MotionPipelineElem& elem)
	{
		// Check if queue is empty
		if (!_pipelinePosn.canGet())
			return false;

		// read the item and remove
		elem = _pipeline[_pipelinePosn._getPos];
		_pipelinePosn.hasGot();
		return true;
	}

	// Remove last element from queue
	bool remove()
	{
		// Check if queue is empty
		if (!_pipelinePosn.canGet())
			return false;

		// remove item
		_pipelinePosn.hasGot();
		return true;
	}

	// Peek the block which would be got (if there is one)
	MotionPipelineElem* peekGet()
	{
		// Check if queue is empty
		if (!_pipelinePosn.canGet())
			return NULL;
		// get pointer to the last item (don't remove)
		return &(_pipeline[_pipelinePosn._getPos]);
	}

	// Peek from the put position
	// 0 is the last element put in the queue
	// 1 is the one put in before that
	// returns NULL when nothing to peek
	MotionPipelineElem* peekNthFromPut(unsigned int N)
	{
		// Get index
		int nthPos = _pipelinePosn.getNthFromPut(N);
		if (nthPos < 0)
			return NULL;
		return &(_pipeline[nthPos]);
	}

	// Peek from the get position
	// 0 is the element next got from the queue
	// 1 is the one got after that
	// returns NULL when nothing to peek
	MotionPipelineElem* peekNthFromGet(unsigned int N)
	{
		// Get index
		int nthPos = _pipelinePosn.getNthFromGet(N);
		if (nthPos < 0)
			return NULL;
		return &(_pipeline[nthPos]);
	}
};
