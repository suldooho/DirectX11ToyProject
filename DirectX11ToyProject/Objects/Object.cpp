#include "Object.h"
#include "../framework.h"

void Object::ExecuteCommandList()
{
	DeviceManager::GetInstance()->GetD3D11ImmediateContext()->ExecuteCommandList(command_list_.Get(), true);
}
