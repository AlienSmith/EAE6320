// Includes
//=========

#include "cMyGame.h"

#include <Engine/Asserts/Asserts.h>
#include <Engine/UserInput/UserInput.h>
#include<Engine/Math/cMatrix_transformation.h>
// Inherited Implementation
//=========================

// Run
//----

void eae6320::cExampleGame::SubmitDataToBeRendered(const float i_elapsedSecondCount_systemTime, const float i_elapsedSecondCount_sinceLastSimulationUpdate)
{
	//Update position
	//background color
	Graphics::SubmitClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	//Camera
	eae6320::Graphics::sCameranPerspective temp;
	temp.kinematic = &m_camera;
	Graphics::SubmitWorldToCameraData(
		Math::cMatrix_transformation::CreateWorldToCameraTransform(
			m_camera.PredictFutureTransform(i_elapsedSecondCount_sinceLastSimulationUpdate)));
	Graphics::SubmitCameraPerspectiveData(Math::cMatrix_transformation::CreateCameraToProjectedTransform_perspective(0.785398f,	1.0f,	0.1f,	100.0f));
	//Object
	Math::cMatrix_transformation trans = m_object.PredictFutureTransform(i_elapsedSecondCount_sinceLastSimulationUpdate);
	Graphics::SubmitdrawCallConstant(m_object.PredictFutureTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
	if (m_showotherobject) {
		Graphics::SubmitEffectWithObject(m_flash_Effect, Graphics::DefaultGeometry::s_manager.Get(m_triangle_handle));
	}
	else {
		Graphics::SubmitEffectWithObject(m_flash_Effect, m_quard);
	}
	//Plane
	Graphics::SubmitdrawCallConstant(m_Plane.PredictFutureTransform(i_elapsedSecondCount_sinceLastSimulationUpdate));
	Graphics::SubmitEffectWithObject(m_flash_Effect, Graphics::DefaultGeometry::s_manager.Get(m_plane_handle));
}

void eae6320::cExampleGame::UpdateSimulationBasedOnTime(const float i_elapsedSecondCount_sinceLastUpdate)
{
	m_camera.position += m_camera.velocity * i_elapsedSecondCount_sinceLastUpdate;
	m_object.position += m_object.velocity * i_elapsedSecondCount_sinceLastUpdate;
}

void eae6320::cExampleGame::UpdateBasedOnInput()
{
	//Make camera trasform
	Math::sVector temp_vol = Math::sVector(0.0f, 0.0f, 0.0f);
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Up)) {
		temp_vol.z = -10.0f;
	}
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Down)) {
		temp_vol.z = 10.0f;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Left)) {
		temp_vol.x = -10.0f;
	}
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::Right)) {
		temp_vol.x = 10.0f;
	}
	m_camera.velocity = temp_vol;
	Math::sVector temp_Object_vol = Math::sVector(0.0f, 0.0f, 0.0f);
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Home)) {
		temp_Object_vol.y = 10.0f;
	}
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::End)) {
		temp_Object_vol.y = -10.0f;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Delete)) {
		temp_Object_vol.x = -10.0f;
	}
	else if (UserInput::IsKeyPressed(UserInput::KeyCodes::PageDown)) {
		temp_Object_vol.x = 10.0f;
	}
	m_object.velocity = temp_Object_vol;
	// Is the user pressing the ESC key?
	if ( UserInput::IsKeyPressed( UserInput::KeyCodes::Escape ) )
	{
		// Exit the application
		const auto result = Exit( EXIT_SUCCESS );
		EAE6320_ASSERT( result );
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Space)) {
		m_showotherobject = true;
	}
	else {
		m_showotherobject = false;
	}
	if (UserInput::IsKeyPressed(UserInput::KeyCodes::Enter)) {
		m_changeeffect = true;
	}
	else {
		m_changeeffect = false;
	}
}

// Initialization / Clean Up
//--------------------------

eae6320::cResult eae6320::cExampleGame::Initialize()
{
	//data/geometry/trangle.geometry
	Graphics::DefaultGeometry::s_manager.Load("data/geometry/triangle.lua", m_triangle_handle);
	Graphics::DefaultGeometry::s_manager.Load("data/geometry/plane.lua", m_plane_handle);
	Graphics::DefaultGeometry::s_manager.Load("data/geometry/sphere.lua", m_sphere_handle);
	Graphics::DefaultGeometry::s_manager.Load("data/geometry/tour.lua", m_tour_handle);
	using namespace Graphics;
	sDataRequriedToIntializeObject squre;
	squre.indexcount = 6;
	squre.vertexcount = 4;
	eae6320::Graphics::VertexFormats::s3dObject temp_vertexData[4];
	{
		temp_vertexData[0].x = 0.0f;
		temp_vertexData[0].y = 0.0f;
		temp_vertexData[0].z = 0.0f;

		temp_vertexData[1].x = 1.0f;
		temp_vertexData[1].y = 0.0f;
		temp_vertexData[1].z = 0.0f;

		temp_vertexData[2].x = 1.0f;
		temp_vertexData[2].y = 1.0f;
		temp_vertexData[2].z = 0.0f;

		temp_vertexData[3].x = 0.0f;
		temp_vertexData[3].y = 1.0f;
		temp_vertexData[3].z = 0.0f;

	}
	squre.vertexData = temp_vertexData;
	uint16_t temp_indexData[6]{ 0,1,2,0,2,3 };
	squre.indexdata = temp_indexData;
	DefaultGeometry::Create(squre, m_quard);

	sDataRequriedToIntializeObject triangle;
	triangle.indexcount = 3;
	triangle.vertexcount = 3;
	eae6320::Graphics::VertexFormats::s3dObject triangle_vertexData[3];
	{

		triangle_vertexData[0].x = 0.0f;
		triangle_vertexData[0].y = 0.0f;
		triangle_vertexData[0].z = 0.0f;

		triangle_vertexData[1].x = 1.0f;
		triangle_vertexData[1].y = 0.0f;
		triangle_vertexData[1].z = 0.0f;

		triangle_vertexData[2].x = 1.0f;
		triangle_vertexData[2].y = 1.0f;
		triangle_vertexData[2].z = 0.0f;

	}							   
	triangle.vertexData = triangle_vertexData;
	uint16_t triangle_indexData[3]{ 0,1,2 };
	triangle.indexdata = triangle_indexData;
	DefaultGeometry::Create(triangle, m_triangle);

	sDataRequriedToIntializeEffect flesh_data;
	flesh_data.vertex_shader_path = "data/Shaders/Vertex/standard.shader";
	flesh_data.fragment_shader_path = "data/Shaders/Fragment/test.shader";
	cEffect::Create(flesh_data, m_flash_Effect);

	sDataRequriedToIntializeEffect white_data;
	white_data.vertex_shader_path = "data/Shaders/Vertex/standard.shader";
	white_data.fragment_shader_path = "data/Shaders/Fragment/standard.shader";
	cEffect::Create(white_data, m_white_Effect);
	int a = sizeof(cEffect);
	int b = sizeof(DefaultGeometry);
	m_camera.position = Math::sVector(0.0f,0.0f,10.0f);
	m_object.position = Math::sVector(0.0f, 0.0f, 5.0f);
	m_Plane.position = Math::sVector(0.0f, -3.0f, 0.0f);
	return Results::Success;
}

eae6320::cResult eae6320::cExampleGame::CleanUp()
{
	Graphics::DefaultGeometry::s_manager.Release(m_triangle_handle);
	Graphics::DefaultGeometry::s_manager.Release(m_plane_handle);
	Graphics::DefaultGeometry::s_manager.Release(m_sphere_handle);
	Graphics::DefaultGeometry::s_manager.Release(m_tour_handle);
	m_flash_Effect->DecrementReferenceCount();
	m_white_Effect->DecrementReferenceCount();
	m_quard->DecrementReferenceCount();
	m_triangle->DecrementReferenceCount();
	m_flash_Effect->DecrementReferenceCount();
	m_white_Effect->DecrementReferenceCount();
	m_quard->DecrementReferenceCount();
	m_triangle->DecrementReferenceCount();
	return Results::Success;
}
