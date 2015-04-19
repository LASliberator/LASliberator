/*
===============================================================================

  FILE:  lasliberateGUI.h

  CONTENTS:

    This tool liberates LiDAR from enslavement by ESRI's closed and proprietary
    lock-in format that is generally known as the "LAZ clone". Read up on the
    controversy via the following links:

  PROGRAMMERS:

    martin.isenburg@rapidlasso.com  -  http://rapidlasso.com

  COPYRIGHT:

    (c) 2015, martin isenburg, rapidlasso - fast tools to catch reality

    This is free software; you can redistribute and/or modify it under the
    terms of the GNU Lesser General Licence as published by the Free Software
    Foundation. See the LICENSE.txt file for more information.

    This software is distributed WITHOUT ANY WARRANTY and without even the
    implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  CHANGE HISTORY:

    19 April 2015 -- simple GUI to free LiDAR enslaved in ESRI's "LAZ clone"

===============================================================================
*/

#pragma once

#include <vcclr.h>
#include <cstdlib>

#include "lasliberate.h"

namespace lasliberateGUI {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for lasliberateGUI
	/// </summary>
	public ref class lasliberateGUI : public System::Windows::Forms::Form
	{
		public:
			lasliberateGUI(void)
			{
				InitializeComponent();
				//
				//TODO: Add the constructor code here
				//
			}

		protected:
			/// <summary>
			/// Clean up any resources being used.
			/// </summary>
			~lasliberateGUI()
			{
				if (components)
				{
					delete components;
				}
			}
		private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
		protected:
		private: System::Windows::Forms::MenuStrip^  menuStrip1;
		private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;

		private: System::Windows::Forms::RadioButton^  create_las;
		private: System::Windows::Forms::RadioButton^  create_laz;

		private: System::Windows::Forms::Button^  liberate;
		private: System::Windows::Forms::TextBox^  textBox1;

		private:
			/// <summary>
			/// Required designer variable.
			/// </summary>
			System::ComponentModel::Container ^components;

	#pragma region Windows Form Designer generated code
			/// <summary>
			/// Required method for Designer support - do not modify
			/// the contents of this method with the code editor.
			/// </summary>
			void InitializeComponent(void)
			{
				this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
				this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
				this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->create_las = (gcnew System::Windows::Forms::RadioButton());
				this->create_laz = (gcnew System::Windows::Forms::RadioButton());
				this->liberate = (gcnew System::Windows::Forms::Button());
				this->textBox1 = (gcnew System::Windows::Forms::TextBox());
				this->menuStrip1->SuspendLayout();
				this->SuspendLayout();
				// 
				// openFileDialog1
				// 
				this->openFileDialog1->Filter = L"imprisoned LAS (*.zlas)|*.zlas";
				this->openFileDialog1->Title = L"Select imprisoned LAS file with *.zlas extension";
				// 
				// menuStrip1
				// 
				this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
				this->menuStrip1->Location = System::Drawing::Point(0, 0);
				this->menuStrip1->Name = L"menuStrip1";
				this->menuStrip1->Size = System::Drawing::Size(227, 24);
				this->menuStrip1->TabIndex = 0;
				this->menuStrip1->Text = L"menuStrip1";
				// 
				// fileToolStripMenuItem
				// 
				this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->openToolStripMenuItem });
				this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
				this->fileToolStripMenuItem->Size = System::Drawing::Size(48, 20);
				this->fileToolStripMenuItem->Text = L"Open";
				// 
				// openToolStripMenuItem
				// 
				this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
				this->openToolStripMenuItem->Size = System::Drawing::Size(92, 22);
				this->openToolStripMenuItem->Text = L"File";
				this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &lasliberateGUI::openToolStripMenuItem_Click);
				// 
				// create_las
				// 
				this->create_las->AutoSize = true;
				this->create_las->Checked = false;
				this->create_las->Location = System::Drawing::Point(14, 36);
				this->create_las->Name = L"create_las";
				this->create_las->Size = System::Drawing::Size(45, 17);
				this->create_las->TabIndex = 2;
				this->create_las->Text = L"LAS";
				this->create_las->UseVisualStyleBackColor = true;
				this->create_las->CheckedChanged += gcnew System::EventHandler(this, &lasliberateGUI::radioButton1_CheckedChanged);
				// 
				// create_laz
				// 
				this->create_laz->AutoSize = true;
				this->create_laz->Checked = true;
				this->create_laz->Location = System::Drawing::Point(14, 60);
				this->create_laz->Name = L"create_laz";
				this->create_laz->Size = System::Drawing::Size(45, 17);
				this->create_laz->TabIndex = 3;
				this->create_laz->TabStop = true;
				this->create_laz->Text = L"LAZ";
				this->create_laz->UseVisualStyleBackColor = true;
				this->create_laz->CheckedChanged += gcnew System::EventHandler(this, &lasliberateGUI::radioButton2_CheckedChanged);
				// 
				// liberate
				// 
				this->liberate->Enabled = false;
				this->liberate->Location = System::Drawing::Point(70, 36);
				this->liberate->Name = L"liberate";
				this->liberate->Size = System::Drawing::Size(98, 41);
				this->liberate->TabIndex = 4;
				this->liberate->Text = L"Liberate !!!";
				this->liberate->UseVisualStyleBackColor = true;
				this->liberate->Click += gcnew System::EventHandler(this, &lasliberateGUI::button_liberate_Click);
				// 
				// textBox1
				// 
				this->textBox1->BackColor = System::Drawing::SystemColors::Menu;
				this->textBox1->BorderStyle = System::Windows::Forms::BorderStyle::None;
				this->textBox1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 27.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
					static_cast<System::Byte>(0)));
				this->textBox1->Location = System::Drawing::Point(182, 32);
				this->textBox1->Margin = System::Windows::Forms::Padding(0);
				this->textBox1->Name = L"smiley";
				this->textBox1->Size = System::Drawing::Size(40, 42);
				this->textBox1->TabIndex = 5;
				this->textBox1->TabStop = false;
				this->textBox1->Text = L"(-;";
				// 
				// lasliberateGUI
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(227, 89);
				this->Controls->Add(this->textBox1);
				this->Controls->Add(this->liberate);
				this->Controls->Add(this->create_laz);
				this->Controls->Add(this->create_las);
				this->Controls->Add(this->menuStrip1);
				this->Name = L"LASliberator";
				this->Text = L"LASliberator";
				this->Load += gcnew System::EventHandler(this, &lasliberateGUI::lasliberateGUI_Load);
				this->menuStrip1->ResumeLayout(false);
				this->menuStrip1->PerformLayout();
				this->ResumeLayout(false);
				this->PerformLayout();

			}
	#pragma endregion
		private: System::Void lasliberateGUI_Load(System::Object^  sender, System::EventArgs^  e) {
		}
		private: System::Void radioButton1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		}
		private: System::Void radioButton2_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
		}
		private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if (this->openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				this->liberate->Enabled = true;
			}
			else
			{
				this->liberate->Enabled = false;
			}
		}
		private: System::Void button_liberate_Click(System::Object^  sender, System::EventArgs^  e)
		{
			if (this->openFileDialog1->FileName->EndsWith(".zlas"))
			{
				// copy file name
				const int cSize = this->openFileDialog1->FileName->Length;

				pin_ptr<const wchar_t> convertedImprisonedFileName = PtrToStringChars(this->openFileDialog1->FileName->ToString());
				const wchar_t *imprisonedFileName = convertedImprisonedFileName;
				char *liberatedFileName = new char[cSize+1];
				wcstombs(liberatedFileName, imprisonedFileName, cSize);
				liberatedFileName[cSize - 4] = 'l';
				liberatedFileName[cSize - 3] = 'a';
				liberatedFileName[cSize - 1] = '\0';
				if (this->create_las->Checked)
				{
					liberatedFileName[cSize - 2] = 's';
					lasliberate(imprisonedFileName, liberatedFileName);
				}
				else
				{
					liberatedFileName[cSize - 2] = 'z';
					lasliberate(imprisonedFileName, liberatedFileName);
				}
				this->liberate->Enabled = false;
				delete[] liberatedFileName;
			}
		}
	};
}
