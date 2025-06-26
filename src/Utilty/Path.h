#pragma once
#include <string>
#include <Windows.h>

/// <summary>
/// �x�s�������ɮ׸��|
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
	/// ��ҼҦ��A�Ω��ഫ��������|
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
			GetModuleFileNameA(NULL, buffer, MAX_PATH);           // ���o������|
			std::string fullPath(buffer);

			// �N '\' �ഫ�� '/'
			for (char& c : fullPath) {
				if (c == '\\') {
					c = '/';
				}
			}

			// ���o�ؿ������]�h���ɮצW�١^
			return fullPath.substr(0, fullPath.find_last_of("/")) + "/";
		}
	};

	Converter* Converter::instance = new Converter();
}