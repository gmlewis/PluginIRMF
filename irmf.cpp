#include "irmf.h"
#include <iostream>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib/httplib.h>
#include <json11/json11.hpp>
#include <pugixml/src/pugixml.hpp>
#include <ghc/filesystem.hpp>

#define BUTTON_SPACE_LEFT -40 * GetDPI()
#define KEYBOARD_TEXTURE_NAME "KeyboardTexture"

namespace irmf
{
	std::string GenerateReadMe(const json11::Json& info, const std::string& linkURL)
	{
		std::string ret = "";

		ret += "Author: " + info["author"].string_value() + "\n";
		ret += "Copyright: " + info["copyright"].string_value() + "\n";
		ret += "Date: " + info["date"].string_value() + "\n";
		ret += "IRMF: " + info["irmf"].string_value() + "\n";
		ret += "Materials: " + info["materials"].string_value() + "\n";
		ret += "Max: " + info["max"].string_value() + "\n";
		ret += "Min: " + info["min"].string_value() + "\n";
		ret += "Notes: " + info["notes"].string_value() + "\n";
		ret += "Options: " + info["options"].string_value() + "\n";
		ret += "Title: " + info["title"].string_value() + "\n";
		ret += "Units: " + info["units"].string_value() + "\n";
		ret += "Version: " + info["version"].string_value() + "\n";
		ret += "Link: " + linkURL + "\n";

		return ret;
	}

	std::string GenerateItems(int index)
	{
		std::string ret =
			"<items>\n"
			"<item name=\"ScreenQuad" + std::to_string(index) + "\" type=\"geometry\">\n"
			"<type>ScreenQuadNDC</type>\n"
			"<width>1</width>\n"
			"<height>1</height>\n"
			"<depth>1</depth>\n"
			"<topology>TriangleList</topology>\n"
			"</item>\n"
			"</items>";
		return ret;
	}

	std::string GenerateVariables()
	{
		std::string ret =
			"<variables>"
			"<variable type=\"float2\" name=\"iResolution\" system=\"ViewportSize\" />"
			"<variable type=\"float\" name=\"iTime\" system=\"Time\" />"
			"<variable type=\"float\" name=\"iTimeDelta\" system=\"TimeDelta\" />"
			"<variable type=\"int\" name=\"iFrame\" system=\"FrameIndex\" />"
			"<variable type=\"float4\" name=\"iMouse\" system=\"MouseButton\" />"
			"</variables>";
		return ret;
	}

	std::string GenerateSettings()
	{
		std::string ret =
			"<entry type=\"camera\" fp=\"false\">"
			"<distance>10</distance>"
			"<pitch>0</pitch>"
			"<yaw>0</yaw>"
			"<roll>0</roll>"
			"</entry>"
			"<entry type=\"clearcolor\" r=\"0\" g=\"0\" b=\"0\" a=\"0\" />"
			"<entry type=\"usealpha\" val=\"false\" />";

		return ret;
	}

	std::string GenerateVertexShader()
	{
// 		const char* vs = R"(#version 330

// layout (location = 0) in vec2 pos;
// layout (location = 1) in vec2 uv;

// out vec2 outUV;

// void main() {
// 	gl_Position = vec4(pos, 0.0, 1.0);
// 	outUV = uv;
// }
// )";

		const char* vs = R"(#version 300 es
out vec4 v_xyz;
void main() {
  gl_Position = projectionMatrix * modelViewMatrix * vec4( position, 1.0 );
  v_xyz = modelMatrix * vec4( position, 1.0 );
}
)";
		return std::string(vs);
	}

	std::string GenerateGLSL(const json11::Json& rpassContainer, const std::string& body)
	{
		std::string ret =
			// "#version 330\n\n"
			// "uniform vec2 iResolution;\n"
			// "uniform float iTime;\n"
			// "uniform float iTimeDelta;\n"
			// "uniform int iFrame;\n"
			// "uniform vec4 iMouse;\n"
			// "uniform sampler2D iChannel0;\n"
			// "uniform sampler2D iChannel1;\n"
			// "uniform sampler2D iChannel2;\n"
			// "uniform sampler2D iChannel3;\n"
			// "out vec4 irmf_outcolor;\n\n"
			"#version 300 es\n"
			"precision highp float;\n"
			"precision highp int;\n"
			"uniform vec3 u_ll;\n"
			"uniform vec3 u_ur;\n"
			"uniform float u_d;\n"
			"uniform int u_numMaterials;\n"
			"uniform vec4 u_color1;\n"
			"uniform vec4 u_color2;\n"
			"uniform vec4 u_color3;\n"
			"uniform vec4 u_color4;\n"
			"uniform vec4 u_color5;\n"
			"uniform vec4 u_color6;\n"
			"uniform vec4 u_color7;\n"
			"uniform vec4 u_color8;\n"
			"uniform vec4 u_color9;\n"
			"uniform vec4 u_color10;\n"
			"uniform vec4 u_color11;\n"
			"uniform vec4 u_color12;\n"
			"uniform vec4 u_color13;\n"
			"uniform vec4 u_color14;\n"
			"uniform vec4 u_color15;\n"
			"uniform vec4 u_color16;\n"
			"in vec4 v_xyz;\n"
			"out vec4 out_FragColor;\n\n"
			+ body + "\n"
			// "void main()\n{\n"
			// "\tmainImage(irmf_outcolor, gl_FragCoord.xy);\n"
			// "}";
			"void main() {\n"
			"	if (any(lessThan(v_xyz.xyz,u_ll))) {\n"
			"		out_FragColor = vec4(0);\n"
			"		// out_FragColor = vec4(0,1,0,1);  // DEBUG\n"
			"		return;\n"
			"	}\n"
			"	if (any(greaterThan(v_xyz.xyz,u_ur))) {\n"
			"		out_FragColor = vec4(0);\n"
			"		// out_FragColor = vec4(0,0,1,1);  // DEBUG\n"
			"		return;\n"
			"	}\n"
			"	vec4 m;\n"
			"	mainModel4(m, v_xyz.xyz);\n"
			"	out_FragColor = %v;\n"
			"	// out_FragColor = v_xyz/5.0 + 0.5;  // DEBUG\n"
			"}\n";

		return ret;
	}

	pugi::xml_document GenerateProject(const json11::Json& rpassContainer, const std::string& body)
	{
		pugi::xml_document doc;
		pugi::xml_node project = doc.append_child("project");
		project.append_attribute("version").set_value(2);

		pugi::xml_node pipelineNode = project.append_child("pipeline");
		pugi::xml_node objectsNode = project.append_child("objects");
		pugi::xml_node settingsNode = project.append_child("settings");

		/////// BUILD RESOURCE LIST ///////
		int index = 0;
		std::vector<std::string> rts;
		std::vector<int> rtIds;
		std::map<int, std::vector<std::pair<std::string, int>>> rtBind;
		std::vector<std::string> textures, textureTypes;
		std::map<std::string, std::vector<std::pair<std::string, int>>> texBinds;

		/////// PIPELINE ///////
		pugi::xml_node node = pipelineNode.append_child("pass");
		node.append_attribute("name").set_value("irmf");
		node.append_attribute("type").set_value("shader");
		node.append_attribute("active").set_value("true");

		pugi::xml_node vsNode = node.append_child("shader");
		vsNode.append_attribute("type").set_value("vs");
		vsNode.append_attribute("path").set_value("shaders/irmfVS.glsl");

		pugi::xml_node psNode = node.append_child("shader");
		psNode.append_attribute("type").set_value("ps");
		psNode.append_attribute("path").set_value("shaders/irmfFS.glsl");

		node.append_child("rendertexture");

		std::string itemsNode = GenerateItems(0);
		node.append_buffer(itemsNode.c_str(), itemsNode.size());

		std::string varNode = GenerateVariables();
		node.append_buffer(varNode.c_str(), varNode.size());

		/////// OBJECTS ///////
		for (int i = 0; i < rts.size(); i++) {
			pugi::xml_node node = objectsNode.append_child("object");
			node.append_attribute("type").set_value("rendertexture");
			node.append_attribute("name").set_value(rts[i].c_str());
			node.append_attribute("rsize").set_value("1.00,1.00");
			node.append_attribute("clear").set_value("true");
			node.append_attribute("r").set_value("0");
			node.append_attribute("g").set_value("0");
			node.append_attribute("b").set_value("0");
			node.append_attribute("a").set_value("1");

			const std::vector<std::pair<std::string, int>>& myBind = rtBind[rtIds[i]];
			for (int j = 0; j < myBind.size(); j++) {
				auto& pair = myBind[j];
				pugi::xml_node bindNode = node.append_child("bind");
				bindNode.append_attribute("slot").set_value(pair.second);
				bindNode.append_attribute("name").set_value(pair.first.c_str());
			}
		}

		/////// SETTINGS ///////
		std::string settings = GenerateSettings();
		settingsNode.append_buffer(settings.c_str(), settings.size());

		return doc;
	}

	void WriteFile(const std::string& filename, const std::string& filedata)
	{
		std::ofstream file(filename);
		file << filedata;
		file.close();
	}

	bool Generate(const std::string& inURL, const std::string& outPath)
	{
		// Examples:
		// https://gmlewis.github.io/irmf-editor/?s=github.com/gmlewis/irmf/blob/master/examples/001-sphere/sphere-1.irmf
		// https://github.com/gmlewis/irmf/blob/master/examples/001-sphere/sphere-1.irmf
		// https://raw.githubusercontent.com/gmlewis/irmf/master/examples/001-sphere/sphere-1.irmf
		const std::string& oldPrefix = "github.com/";
		std::string irmfURL = inURL;
		size_t startGitHub = inURL.find(oldPrefix);
		if (startGitHub != std::string::npos) {
			irmfURL = "https://raw.githubusercontent.com/" + inURL.substr(startGitHub+oldPrefix.length());
			size_t startBlob = irmfURL.find("/blob/");
			if (startBlob != std::string::npos) {
				irmfURL.replace(startBlob, 6, "/");
			}
		} else if (irmfURL.find("raw.githubusercontent.com") == std::string::npos) {
			return false;
		}

		httplib::SSLClient cli("raw.githubusercontent.com");

		auto res = cli.Get(irmfURL.c_str());

		if (!res || res->status != 200) {
			return false;
		}

		if (res->body.substr(0, 3) != "/*{") {
			std::cerr << "IRMF shader must start with: '/*{'" << std::endl;
			return false;
		}

		size_t endJSON = res->body.find("}*/");
		if (endJSON == std::string::npos) {
			std::cerr << "IRMF shader must have JSON preamble ending with: '}*/'" << std::endl;
			return false;
		}

		std::string jsonBody = res->body.substr(2, endJSON-1);

		std::string err;
		json11::Json jdata = json11::Json::parse(jsonBody, err);

		if (err != "") {
			std::cerr << "JSON parsing failed: " << err << std::endl;
			return false;
		}

		if (jdata["Error"].is_string()) {
			std::cerr << "JSON parsing failed: " << jdata["Error"].string_value() << std::endl;
			return false;
		}

		if (!ghc::filesystem::exists(outPath)) {
			ghc::filesystem::create_directories(outPath);
		}

		std::string shadersDir = outPath + "/shaders";
		if (!ghc::filesystem::exists(shadersDir)) {
			ghc::filesystem::create_directories(shadersDir);
		}

		// README.txt
		WriteFile(outPath + "/README.txt", GenerateReadMe(jdata, inURL));

		// project.sprj
		pugi::xml_document doc = GenerateProject(jdata, res->body);
		std::ofstream sprjFile(outPath + "/project.sprj");
		doc.print(sprjFile);
		sprjFile.close();

		// shaders
		std::string shaderPath = outPath + "/shaders/irmfFS.glsl";
		WriteFile(shaderPath, GenerateGLSL(jdata, res->body));
		WriteFile(outPath + "/shaders/irmfVS.glsl", GenerateVertexShader());

		return err.size() == 0;
	}

	bool IRMF::Init(bool isWeb, int sedVersion) {
		m_isPopupOpened = false;

		if (sedVersion == 1003005)
			m_hostVersion = 1;
		else
			m_hostVersion = GetHostIPluginMaxVersion();

		return true;
	}

	void IRMF::InitUI(void* ctx)
	{
		ImGui::SetCurrentContext((ImGuiContext*)ctx);
	}

	void IRMF::Update(float delta)
	{
		// ##### UNIFORM MANAGER POPUP #####
		if (m_isPopupOpened) {
			ImGui::OpenPopup("Import IRMF shader##irmf_import");
			m_error = "";
			m_isPopupOpened = false;
		}
		ImGui::SetNextWindowSize(ImVec2(530, 160), ImGuiCond_Once);
		if (ImGui::BeginPopupModal("Import IRMF shader##irmf_import")) {
			ImGui::Text("IRMF link:"); ImGui::SameLine();
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##irmf_link_insert", m_link, 256);
			ImGui::PopItemWidth();

			ImGui::Text("Project path:"); ImGui::SameLine();
			ImGui::PushItemWidth(BUTTON_SPACE_LEFT);
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			ImGui::InputText("##pui_vspath", m_path, MY_PATH_LENGTH);
			ImGui::PopItemFlag();
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("...##pui_vsbtn", ImVec2(-1, 0)) && m_hostVersion >= 2)
				ImGuiDirectoryDialogOpen("irmfLocationDlg", "Save location");

			if (m_hostVersion >= 2 && ImGuiFileDialogIsDone("irmfLocationDlg")) {
				if (ImGuiFileDialogGetResult())
					ImGuiFileDialogGetPath(m_path);

				ImGuiFileDialogClose("irmfLocationDlg");
			}


			if (!m_errorOccured)
				ImGui::NewLine();
			else
				ImGui::Text("[ERROR] %s", m_error.c_str());


			if (ImGui::Button("OK")) {
				std::string irmfLink = m_link;
				std::string errMessage = "";
				// Examples:
				// https://gmlewis.github.io/irmf-editor/?s=github.com/gmlewis/irmf/blob/master/examples/001-sphere/sphere-1.irmf
				// https://github.com/gmlewis/irmf/blob/master/examples/001-sphere/sphere-1.irmf
				// https://raw.githubusercontent.com/gmlewis/irmf/master/examples/001-sphere/sphere-1.irmf
				if (irmfLink.find("github.com") == std::string::npos &&
						irmfLink.find("raw.githubusercontent.com") == std::string::npos) {
					errMessage = "Please insert correct IRMF shader link from GitHub.";
				}
				if (irmfLink.find(".irmf") != irmfLink.length()-5) {
					errMessage = "IRMF shader link must end in '.irmf'";
				}

				if (errMessage.size() == 0) {
					std::string outPath(m_path);

					if (outPath.size() == 0)
						errMessage = "Please set the output path.";
					else {
						bool res = Generate(irmfLink, outPath);
						if (!res)
							errMessage = "Could not find IRMF shader.";
						else
							OpenProject(UI, (outPath + "/project.sprj").c_str());
					}
				}

				m_error = errMessage;
				m_errorOccured = (m_error.size() != 0);

				if (m_error.size() == 0)
					ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
	}

	bool IRMF::HasMenuItems(const char* name)
	{
		return strcmp(name, "file") == 0;
	}

	void IRMF::ShowMenuItems(const char* name)
	{
		if (strcmp(name, "file") == 0) {
			if (ImGui::Selectable("Import IRMF shader")) {
				m_isPopupOpened = true;
			}
		}
	}
}
