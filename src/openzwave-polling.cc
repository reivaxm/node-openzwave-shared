/*
* Copyright (c) 2013 Jonathan Perkin <jonathan@perkin.org.uk>
* Copyright (c) 2015 Elias Karakoulakis <elias.karakoulakis@gmail.com>
*
* Permission to use, copy, modify, and distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "openzwave.hpp"
using namespace v8;
using namespace node;

namespace OZW {


	/* Set the time period between polls of a node's state. Due to patent
	 * concerns, some devices do not report state changes automatically to
	 * the controller. These devices need to have their state polled at
	 * regular intervals. The length of the interval is the same for all
	 * devices. To even out the Z-Wave network traffic generated by
	 * polling, OpenZWave divides the polling interval by the number of
	 * devices that have polling enabled, and polls each in turn.
	 * It is recommended that if possible, the interval should not be set
	 * shorter than the number of polled devices in seconds (so that the
	 * network does not have to cope with more than one poll per second).
	 */
	// ===================================================================
	NAN_METHOD(OZW::SetPollInterval) 
	// ===================================================================
	{
		Nan::HandleScope scope;

		uint32 intervalMillisecs = info[0]->ToNumber()->Value();
		OpenZWave::Manager::Get()->SetPollInterval (intervalMillisecs, false);
	}


	/*
	* Enable/Disable polling on a COMMAND_CLASS basis.
	*/
	// ===================================================================
	NAN_METHOD(OZW::EnablePoll)
	// ===================================================================
	{
		Nan::HandleScope scope;

		uint8 nodeid = info[0]->ToNumber()->Value();
		uint8 comclass = info[1]->ToNumber()->Value();
		uint8 intensity = (info.Length() > 2) ? info[2]->ToNumber()->Value() : 1;

		NodeInfo *node;
		std::list<OpenZWave::ValueID>::iterator vit;

		if ((node = get_node_info(nodeid))) {
			for (vit = node->values.begin(); vit != node->values.end(); ++vit) {
				if ((*vit).GetCommandClassId() == comclass) {
					OpenZWave::Manager::Get()->EnablePoll((*vit), intensity);
					break;
				}
			}
		}
	}

	// ===================================================================
	NAN_METHOD(OZW::DisablePoll)
	// ===================================================================
	{
		Nan::HandleScope scope;

		uint8 nodeid = info[0]->ToNumber()->Value();
		uint8 comclass = info[1]->ToNumber()->Value();
		NodeInfo *node;
		std::list<OpenZWave::ValueID>::iterator vit;

		if ((node = get_node_info(nodeid))) {
			for (vit = node->values.begin(); vit != node->values.end(); ++vit) {
				if ((*vit).GetCommandClassId() == comclass) {
					OpenZWave::Manager::Get()->DisablePoll((*vit));
					break;
				}
			}
		}
	}

	// Determine the polling of a device's state.
	// ===================================================================
	NAN_METHOD(OZW::IsPolled) { OZWMGRGetByZWaveValueId(isPolled, Boolean) }
	// ===================================================================

	// ===================================================================
	NAN_METHOD(OZW::SetPollIntensity)
	// ===================================================================
	{
		Nan::HandleScope scope;

		uint8 nodeid = info[0]->ToNumber()->Value();
		uint8 comclass = info[1]->ToNumber()->Value();
		uint8 instance = info[2]->ToNumber()->Value();
		uint8 index = info[3]->ToNumber()->Value();
		uint8 intensity = info[4]->ToNumber()->Value();

		NodeInfo *node;
		std::list<OpenZWave::ValueID>::iterator vit;

		if ((node = get_node_info(nodeid))) {
			for (vit = node->values.begin(); vit != node->values.end(); ++vit) {
				if (((*vit).GetCommandClassId() == comclass) && ((*vit).GetInstance() == instance) && ((*vit).GetIndex() == index)) {

					OpenZWave::Manager::Get()->SetPollIntensity (*vit, intensity);

				}
			}
		}
	}

 	// Get the polling intensity of a device's state.
	// ===================================================================
	NAN_METHOD(OZW::GetPollIntensity)
	// ===================================================================
	{
		Nan::HandleScope scope;

		uint8 nodeid = info[0]->ToNumber()->Value();
		uint8 comclass = info[1]->ToNumber()->Value();
		uint8 instance = info[2]->ToNumber()->Value();
		uint8 index = info[3]->ToNumber()->Value();

		NodeInfo *node;
		std::list<OpenZWave::ValueID>::iterator vit;

		if ((node = get_node_info(nodeid))) {
			for (vit = node->values.begin(); vit != node->values.end(); ++vit) {
				if (((*vit).GetCommandClassId() == comclass) && ((*vit).GetInstance() == instance) && ((*vit).GetIndex() == index)) {
					uint8 i = OpenZWave::Manager::Get()->GetPollIntensity(*vit);
					info.GetReturnValue().Set(Nan::New<Integer>(i));
				}
			}
		}
	}
}
