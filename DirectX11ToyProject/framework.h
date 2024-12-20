﻿// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <wrl.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <tchar.h>
#include <memory>
#include <cwchar>
#include <string> 

#include <d3d11.h> 
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include "DirectXTex.h"

#include "Core/DebugLog.h"
#include "Core/TimerManager.h"
#include "Core/DeviceManager.h"
#include "Core/SwapChainManager.h"
#include "FrameResources/FrameResourcesManager.h"
#include "Meshes/MeshesManager.h"
#include "Objects/ObjectsManager.h"
#include "Objects/LightsManager.h"
