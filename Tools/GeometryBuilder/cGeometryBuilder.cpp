#include "cGeometryBuilder.h"
#include "Engine/Platform/Platform.h"
#include "Tools/AssetBuildLibrary/Functions.h"
eae6320::cResult eae6320::Assets::cGeometryBuilder::Build(const std::vector<std::string>& i_arguments)
{

	eae6320::cResult result = Platform::CopyFileW(m_path_source, m_path_target, false, true);
	if (!result.IsSuccess()) {
		eae6320::Assets::OutputErrorMessageWithFileInfo(m_path_source, "Build Geoemtry failed");
	}
	return result;
}