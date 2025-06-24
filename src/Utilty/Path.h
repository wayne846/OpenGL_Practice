#pragma once
#include <string>
#include <Windows.h>

namespace Path {
	namespace Shader {
		const std::string STANDARD_VERT = "assets/shaders/Standard.vert";
		const std::string STANDARD_FRAG = "assets/shaders/Standard.frag";
	}

	namespace Model {
		const std::string SPHERE = "assets/models/Sphere.obj";
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