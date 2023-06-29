#include "Application.h"

#include "Win32RenderWindow.h"
#include "RendererDX11.h"

using namespace Glyph3;

namespace LJMUDX
{
	//////////////////////////////////////
	//LJMUDX11APPS04.H
	//Class Application for a DirectX 11
	//Driven Application using the DirectX Toolkit
	//Hieroglyph 3 Rendering Engine and LUA.
	//
	//This is Tutorial Session 04 of 5002MATH
	//
	//AUTHORS:  DR CHRIS CARTER
	//			MR MIKE BASKETT
	//////////////////////////////////////
	class LJMUDX11AppS04 : public Application //Inherit from the Hieroglyph Base Class
	{

	public:
		//------------CONSTRUCTORS------------------------------------------------
		LJMUDX11AppS04();	//Standard Empty Constructor which builds the object

	public:
		//------------INHERITED METHODS-------------------------------------------
		virtual void Initialize();	//Initialise the DirectX11 Scene
		virtual void Update();		//Update the DirectX Scene
		virtual void Shutdown();	//Shutdown the DirectX11 Scene

		virtual bool ConfigureEngineComponents();	//Initialise Hieroglyph and DirectX TK Modules
		virtual void ShutdownEngineComponents();	//Destroy Hieroglyph and DirectX TK Modules

		virtual void TakeScreenShot();				//Allow a screenshot to be generated

		virtual bool HandleEvent(EventPtr pEvent);	//Handle an I/O Event
		virtual std::wstring GetName();				//Get the Name of this App Instance

		//------------CUSTOM METHODS-----------------------------------------------
		void inputAssemblyStage();					//Stage to setup our VB and IB Info

	    protected:
		//-------------CLASS MEMBERS-----------------------------------------------
		RendererDX11*			_obj_renderer11;	//Pointer to our DirectX 11 Device
		Win32RenderWindow*		_obj_window;		//Pointer to our Windows-Based Window

		int						_swap_index;		//Index of our Swap Chain 
		ResourcePtr				_tgt_render;		//Pointer to the GPU Render Target for Colour
		ResourcePtr				_tgt_depth;			//Pointer to the GPU Render Target for Depth

		ResourcePtr             _buff_verts;		//Pointer to the GPU Stored Vertex Buffer
		int                     _layout;			//Integer Representing the used Memory Layout
		ResourcePtr             _buff_index;		//Pointer to the GPU Stored Index Buffer
		RenderEffectDX11		_shader_basic;		//Pointer to the Programmable Pipeline Shader Used

		Matrix4f				_mat_world;			//World Matrix of our Object
		Matrix4f				_mat_view;			//View Matrix of our Object
		Matrix4f				_mat_proj;			//Projection Matrix of our Object
	};

}