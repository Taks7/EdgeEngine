//#include "AboutMenu.h"
//#include "ImGui/imgui.h"
//AboutMenu::AboutMenu()
//{
//	active = false;
//}
//
//AboutMenu::~AboutMenu()
//{
//	//https://www.youtube.com/watch?v=9Gognvp8BvQ&ab_channel=OneShot mirar para repasar cosas de ImGuI
//}
//
//void AboutMenu::Draw()
//{
//	ImGui::SetNextWindowBgAlpha(1.0f);
//	ImGui::Begin("About");
//	{
//		ImGui::SetWindowPos({ float(App->window->screen_surface->h - 130),20 });
//		ImGui::SetWindowSize({ 600,700 });
//		ImGui::Text("Edge Engine");
//		ImGui::Text("Next world revolution engine!");
//		ImGui::Text("By Albert Ramisa and Alejandro Martin");
//		ImGui::NewLine();
//		ImGui::Text("3rd parties libraries used:");
//		//AQUI PONER TODAS LAS LIBRERIAS (hay que mirar)
//		ImGui::BulletText("SDL 2.0.4");
//		ImGui::BulletText("MathGeoLib 1.5");
//		ImGui::BulletText("Json 1.2.1");
//		ImGui::BulletText("ImGui 1.89");
//		ImGui::BulletText("Glew 2.2.0");
//		ImGui::BulletText("OpenGL 2.0");
//		//
//		ImGui::NewLine();
//		ImGui::Text("MIT License");
//		ImGui::NewLine();
//		ImGui::Text("Copyright (c) 2012 - 2021 Krzysztof Gabis");
//		ImGui::NewLine();
//		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a");
//		ImGui::Text("copy of this software and associated documentation files(the 'Software'), to dealin"); 
//		ImGui::Text("the Software without restriction, including without limitation the rights"); 
//		ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and /or sell"); 
//		ImGui::Text("copies of the Software, and to permit persons to whom the Software is ");
//		ImGui::Text("furnished to do so, subject to the following conditions : ");
//		ImGui::NewLine();
//		ImGui::Text("The above copyright notice and this permission notice shall be included in");
//		ImGui::Text("all copies or substantial portions of the Software.");
//		ImGui::NewLine();
//		ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
//		ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
//		ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
//		ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
//		ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
//		ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN");
//		ImGui::Text("THE SOFTWARE.");
//	}
//	ImGui::End();
//
//}