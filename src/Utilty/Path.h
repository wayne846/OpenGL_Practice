#pragma once
#include <string>
#include <Windows.h>

/// <summary>
/// 儲存素材的檔案路徑
/// </summary>
namespace Path {
	namespace Shader {
		const std::string STANDARD_VERT = "assets/shaders/Standard.vert";
		const std::string STANDARD_FRAG = "assets/shaders/Standard.frag";
	}

	namespace Model {
		const std::string SPHERE = "assets/models/Sphere.obj";
	}

	namespace Texture {
		const std::string BRICKWALL = "assets/textures/brickwall.jpg";
		const std::string BRICKWALL_NORMAL = "assets/textures/brickwall_normal.jpg";

		const std::string BADLANDS = "assets/textures/badlands_albedo.png";
		const std::string BADLANDS_NORMAL = "assets/textures/badlands_normal.png";

		const std::string PLASTER = "assets/textures/Plaster_Color.jpg";
		const std::string PLASTER_NORMAL = "assets/textures/Plaster_Normal.jpg";
	}
	
	/// <summary>
	/// 單例模式，用於轉換成絕對路徑
	/// </summary>
	class Converter {
	public:
		static Converter* GetInstance() {
			return instance;
		}

		std::string ExeDir() {
			return exeDir;
		}

		std::string ToAbsolutePath(std::string path) {
			return exeDir + path;
		}

	private:
		static Converter* instance;

		std::string exeDir;

		Converter() {
			exeDir = GetExeDir();
		}

		

		std::string GetExeDir() {
			char buffer[MAX_PATH];
			GetModuleFileNameA(NULL, buffer, MAX_PATH);           // 取得完整路徑
			std::string fullPath(buffer);

			// 將 '\' 轉換為 '/'
			for (char& c : fullPath) {
				if (c == '\\') {
					c = '/';
				}
			}

			// 取得目錄部分（去除檔案名稱）
			return fullPath.substr(0, fullPath.find_last_of("/")) + "/";
		}
	};

	Converter* Converter::instance = new Converter();
}