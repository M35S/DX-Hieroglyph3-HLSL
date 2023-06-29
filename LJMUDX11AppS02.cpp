//------------Include the Application Header File----------------------------
#include "LJMUDX11AppS02.h"
//------------DX TK AND STD/STL Includes-------------------------------------
#include <sstream>
#include <DirectXMath.h>
//------------Include Hieroglyph Engine Files--------------------------------
//Include the Logging System
#include "Log.h"
//Include the Event System
#include "EventManager.h"
#include "EvtFrameStart.h"
#include "EvtChar.h"
#include "EvtKeyUp.h"
#include "EvtKeyDown.h"
#include "ScriptManager.h"
//Include the DirectX Rendering Components
#include "PipelineManagerDX11.h"
#include "BlendStateConfigDX11.h"
#include "BufferConfigDX11.h"
#include "DepthStencilStateConfigDX11.h"
#include "RasterizerStateConfigDX11.h"
#include "SwapChainConfigDX11.h"
#include "Texture2dConfigDX11.h"

//Add a Using Directive to avoid typing Glyph3 for basic constructs
using namespace Glyph3;
//Include our own application Namespace
using namespace LJMUDX;

//--------GLOBAL VARIABLES AND STRUCTURE DEFINITIONS-------------------------
struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

LJMUDX11AppS04 AppInstance; 

//---------CONSTRUCTORS-------------------------------------------------------
LJMUDX11AppS04::LJMUDX11AppS04()
{
}

//---------METHODS------------------------------------------------------------

//////////////////////////////////////
// Get the Window Name of this Application
//////////////////////////////////////
std::wstring LJMUDX11AppS04::GetName()
{
	return(std::wstring(L"5002MATH: Session04 - Low Level DX11"));
}

/////////////////////////////////////
// Assemble our Input Layouts for this
// Stage of the Pipeline.
/////////////////////////////////////
void LJMUDX11AppS04::inputAssemblyStage()
{
	//Specify our Vertex Data 
	D3D11_INPUT_ELEMENT_DESC tdesc[] =
	{
		{ "SV_POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	
	std::vector<D3D11_INPUT_ELEMENT_DESC> tlayout;
	tlayout.push_back(tdesc[0]);
	tlayout.push_back(tdesc[1]);
	
	this->_layout	= this->_obj_renderer11->CreateInputLayout(tlayout,this->_shader_basic.GetVertexShader());
	if (this->_layout == -1)
	{
		Log::Get().Write(L"Failed to create vertex layout");
		assert(false);
	}
}

////////////////////////////////////
// Initialise our DirectX 3D Scene
////////////////////////////////////
void LJMUDX11AppS04::Initialize()
{
	// create the vertex buffer resource
	{
		Vertex tvertices[] =
		{
			{ DirectX::XMFLOAT3(-1.5f, 1.5f, -1.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(1.5f, 1.5f, -1.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(1.5f, 1.5f, 1.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-1.5f, 1.5f, 1.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-1.5f, -1.5f, -1.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(1.5f, -1.5f, -1.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
			{ DirectX::XMFLOAT3(1.5f, -1.5f, 1.5f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
			{ DirectX::XMFLOAT3(-1.5f, -1.5f, 1.5f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		};
		D3D11_SUBRESOURCE_DATA tdata;
		tdata.pSysMem			= tvertices;
		tdata.SysMemPitch		= 0;
		tdata.SysMemSlicePitch	= 0;

		BufferConfigDX11 tconfig_vb;
		tconfig_vb.SetDefaultVertexBuffer(8 * sizeof(Vertex), false);
		this->_buff_verts = this->_obj_renderer11->CreateVertexBuffer( &tconfig_vb, &tdata);
		if(this->_buff_verts->m_iResource == -1 )
		{
			Log::Get().Write( L"Failed to create vertex buffer");   
			assert(false);
		}
	}
	{
		UINT tindices[] =
		{
			3,1,0,
			2,1,3,

			0,5,4,
			1,5,0,

			3,4,7,
			0,4,3,

			1,6,5,
			2,6,1,

			2,7,6,
			3,7,2,

			6,4,5,
			7,4,6,
		};
		D3D11_SUBRESOURCE_DATA tdata;
		tdata.pSysMem			= tindices;
		tdata.SysMemPitch		= 0;
		tdata.SysMemSlicePitch	= 0;

		BufferConfigDX11 tconfig_ib;
		tconfig_ib.SetDefaultIndexBuffer(sizeof(UINT)* 36, false);
		
		this->_buff_index = this->_obj_renderer11->CreateIndexBuffer(&tconfig_ib, &tdata);
		if(this->_buff_index->m_iResource == -1 ) 
		{
			Log::Get().Write( L"Failed to create index buffer" );   
			assert(false);
		}
	}

	//Call the Input Assembly Stage to setup the layout of our Engine Objects
	this->inputAssemblyStage();

	//Create the world matrix for our Object
	this->_mat_world = Matrix4f::Identity();

	// Create the view matrix
	DirectX::XMVECTOR tvect_eye = DirectX::XMVectorSet( 0.0f, 1.0f, -5.0f, 0.0f );
	DirectX::XMVECTOR tvect_rgt = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	DirectX::XMVECTOR tvect_up = DirectX::XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	this->_mat_view = *((Matrix4f*)&DirectX::XMMatrixLookAtLH(tvect_eye,tvect_rgt,tvect_up));

	// Create the projection matrix
	FLOAT tres_width	= (FLOAT)this->_obj_window->GetWidth();
	FLOAT tres_height	= (FLOAT)this->_obj_window->GetHeight();
	this->_mat_proj		= *((Matrix4f*)&DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2,tres_width/tres_height,0.01f,100.0f));

	// Engine will automatically keep all concat'd matrices up to date
	this->_obj_renderer11->m_pParamMgr->SetWorldMatrixParameter(&this->_mat_world);
	this->_obj_renderer11->m_pParamMgr->SetViewMatrixParameter(&this->_mat_view);
	this->_obj_renderer11->m_pParamMgr->SetProjMatrixParameter(&this->_mat_proj);
}

///////////////////////////////////
// Update the State of our Game and 
// Output the Results to Screen (Render)
/////////////////////////////////// 
void LJMUDX11AppS04::Update()
{
	this->m_pTimer->Update();
	EvtManager.ProcessEvent(EvtFrameStartPtr( new EvtFrameStart(this->m_pTimer->Elapsed())));

	// Clear the window
	this->_obj_renderer11->pImmPipeline->ClearBuffers( Vector4f( 0.0f, 0.0f, 0.0f, 0.0f ), 1.0f );
	
	//----------START RENDERING--------------------------------------------------------------

	//Measure the time per frame
	static float ttime_diff = 0.0f;
	static DWORD ttime_start = 0;
	DWORD ttime_cur = GetTickCount();
	if (ttime_start == 0)
	{
		ttime_start = ttime_cur;
	}
	ttime_diff = (ttime_cur - ttime_start) / 1000.0f;

	this->_mat_world = Matrix4f::RotationMatrixY(ttime_diff) * Matrix4f::RotationMatrixX(ttime_diff);
	this->_obj_renderer11->m_pParamMgr->SetWorldMatrixParameter(&this->_mat_world);

	// Perform the actual drawing
	UINT tstride = sizeof(Vertex);
	this->_obj_renderer11->pImmPipeline->Draw(this->_shader_basic, 
										      this->_buff_verts,this->_buff_index,this->_layout, 
											  D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, 
											  tstride, 36, 
											  this->_obj_renderer11->m_pParamMgr);

	//--------END RENDERING-------------------------------------------------------------
	this->_obj_renderer11->Present(this->_obj_window->GetHandle(), this->_obj_window->GetSwapChain());
}

///////////////////////////////////
// Configure the DirectX 11 Programmable
// Pipeline Stages and Create the Window
// Calls 
///////////////////////////////////
bool LJMUDX11AppS04::ConfigureEngineComponents()
{
	// The application currently supplies the 
	int twidth = 1024;
	int theight = 768;

	// Set the render window parameters and initialize the window
	this->_obj_window = new Win32RenderWindow();
	this->_obj_window->SetPosition(25, 25);
	this->_obj_window->SetSize(twidth, theight);
	this->_obj_window->SetCaption(this->GetName());
	this->_obj_window->Initialize(this);


	// Create the renderer and initialize it for the desired device
	// type and feature level.
	this->_obj_renderer11 = new RendererDX11();

	if (!this->_obj_renderer11->Initialize(D3D_DRIVER_TYPE_HARDWARE, D3D_FEATURE_LEVEL_11_0))
	{
		Log::Get().Write(L"Could not create hardware device, trying to create the reference device...");

		if (!this->_obj_renderer11->Initialize(D3D_DRIVER_TYPE_REFERENCE, D3D_FEATURE_LEVEL_10_0))
		{
			ShowWindow(this->_obj_window->GetHandle(), SW_HIDE);
			MessageBox(this->_obj_window->GetHandle(), L"Could not create a hardware or software Direct3D 11 device!", L"5002 MATH - Session 04", MB_ICONEXCLAMATION | MB_SYSTEMMODAL);
			this->RequestTermination();
			return(false);
		}

		// If using the reference device, utilize a fixed time step for any animations.
		this->m_pTimer->SetFixedTimeStep(1.0f / 10.0f);
	}

	// Create a swap chain for the window that we started out with.  This
	// demonstrates using a configuration object for fast and concise object
	// creation.
	SwapChainConfigDX11 tconfig;
	tconfig.SetWidth(this->_obj_window->GetWidth());
	tconfig.SetHeight(this->_obj_window->GetHeight());
	tconfig.SetOutputWindow(this->_obj_window->GetHandle());
	this->_swap_index = this->_obj_renderer11->CreateSwapChain(&tconfig);
	this->_obj_window->SetSwapChain(this->_swap_index);


	//Create Colour and Depth Buffers
	this->_tgt_render = this->_obj_renderer11->GetSwapChainResource(this->_swap_index);

	Texture2dConfigDX11 tdepthconfig;
	tdepthconfig.SetDepthBuffer(twidth, theight);
	this->_tgt_depth = this->_obj_renderer11->CreateTexture2D(&tdepthconfig, 0);

	// Bind the swap chain render target and the depth buffer for use in rendering.  
	this->_obj_renderer11->pImmPipeline->ClearRenderTargets();
	this->_obj_renderer11->pImmPipeline->OutputMergerStage.DesiredState.RenderTargetViews.SetState(0, this->_tgt_render->m_iResourceRTV);
	this->_obj_renderer11->pImmPipeline->OutputMergerStage.DesiredState.DepthTargetViews.SetState(this->_tgt_depth->m_iResourceDSV);
	this->_obj_renderer11->pImmPipeline->ApplyRenderTargets();

	this->_shader_basic.SetVertexShader(this->_obj_renderer11->LoadShader(
		VERTEX_SHADER,
		std::wstring(L"RotatingCube.hlsl"),
		std::wstring(L"VSMain"),
		std::wstring(L"vs_4_0"),
		true));

	this->_shader_basic.SetGeometryShader(this->_obj_renderer11->LoadShader(
		GEOMETRY_SHADER,
		std::wstring(L"RotatingCube.hlsl"),
		std::wstring(L"GSMain"),
		std::wstring(L"gs_4_0"),
		true));

	this->_shader_basic.SetPixelShader(
		this->_obj_renderer11->LoadShader(PIXEL_SHADER,
		std::wstring(L"RotatingCube.hlsl"),
		std::wstring(L"PSMain"),
		std::wstring(L"ps_4_0"),
		true));

	DepthStencilStateConfigDX11 tstencilconfig;
	int tstate_ds = this->_obj_renderer11->CreateDepthStencilState(&tstencilconfig);
	if (tstate_ds == -1)
	{
		Log::Get().Write(L"Failed to create light depth stencil state");
		assert(false);
	}

	BlendStateConfigDX11 tblendconfig;
	int tstate_bs = this->_obj_renderer11->CreateBlendState(&tblendconfig);
	if (tstate_bs == -1)
	{
		Log::Get().Write(L"Failed to create light blend state");
		assert(false);
	}

	RasterizerStateConfigDX11 tconfig_rs;
	tconfig_rs.CullMode = D3D11_CULL_BACK;
	int tstate_rs = this->_obj_renderer11->CreateRasterizerState(&tconfig_rs);
	if (tstate_rs == -1)
	{
		Log::Get().Write(L"Failed to create light rasterizer state");
		assert(false);
	}

	//Setup the Non-Programmable Stages of the Pipeline and the State Objects
	this->_shader_basic.m_iBlendState = tstate_bs;
	this->_shader_basic.m_iDepthStencilState = tstate_ds;
	this->_shader_basic.m_iRasterizerState = tstate_rs;
	this->_shader_basic.m_uStencilRef = tstate_ds;

	D3D11_VIEWPORT tviewport;
	tviewport.Width = static_cast< float >(twidth);
	tviewport.Height = static_cast< float >(theight);
	tviewport.MinDepth = 0.0f;
	tviewport.MaxDepth = 1.0f;
	tviewport.TopLeftX = 0;
	tviewport.TopLeftY = 0;

	int tvpindex = this->_obj_renderer11->CreateViewPort(tviewport);
	this->_obj_renderer11->pImmPipeline->RasterizerStage.DesiredState.ViewportCount.SetState(1);
	this->_obj_renderer11->pImmPipeline->RasterizerStage.DesiredState.Viewports.SetState(0, tvpindex);
	return(true);
}

//////////////////////////////////
//Handle Input Events in the Application
//////////////////////////////////
bool LJMUDX11AppS04::HandleEvent(EventPtr pevent)
{
	eEVENT e = pevent->GetEventType();

	if (e == SYSTEM_KEYBOARD_KEYDOWN)
	{
		EvtKeyDownPtr tkey_down = std::static_pointer_cast<EvtKeyDown>(pevent);
		unsigned int  tkeycode = tkey_down->GetCharacterCode();
	}
	else if (e == SYSTEM_KEYBOARD_KEYUP)
	{
		EvtKeyUpPtr tkey_up = std::static_pointer_cast<EvtKeyUp>(pevent);
		unsigned int tkeycode = tkey_up->GetCharacterCode();
	}

	return(Application::HandleEvent(pevent));
}

//////////////////////////////////
// Destroy Resources created by the engine
//////////////////////////////////
void LJMUDX11AppS04::ShutdownEngineComponents()
{
	if (this->_obj_renderer11)
	{
		this->_obj_renderer11->Shutdown();
		delete this->_obj_renderer11;
	}

	if (this->_obj_window)
	{
		this->_obj_window->Shutdown();
		delete this->_obj_window;
	}
}

//////////////////////////////////
// Shutdown the Application
//////////////////////////////////
void LJMUDX11AppS04::Shutdown()
{
	//NOTHING TO DO HERE
}

//////////////////////////////////
// Take a Screenshot of the Application
//////////////////////////////////
void LJMUDX11AppS04::TakeScreenShot()
{
	if (this->m_bSaveScreenshot)
	{
		this->m_bSaveScreenshot = false;
		this->_obj_renderer11->pImmPipeline->SaveTextureScreenShot(0, this->GetName());
	}
}
