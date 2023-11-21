#pragma once
#include "WindowChat.h"


namespace CppCLRWinFormsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
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
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ ipLabel;
	private: System::Windows::Forms::TextBox^ ipTextBox;
	private: System::Windows::Forms::TextBox^ portTextBox;
	private: System::Windows::Forms::Label^ portLabel;
	private: System::Windows::Forms::TextBox^ usernameTextBox;
	private: System::Windows::Forms::Label^ usernameLabel;
	private: System::Windows::Forms::Button^ submitButton;
	protected:

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
			this->ipLabel = (gcnew System::Windows::Forms::Label());
			this->ipTextBox = (gcnew System::Windows::Forms::TextBox());
			this->portLabel = (gcnew System::Windows::Forms::Label());
			this->portTextBox = (gcnew System::Windows::Forms::TextBox());
			this->usernameLabel = (gcnew System::Windows::Forms::Label());
			this->usernameTextBox = (gcnew System::Windows::Forms::TextBox());
			this->submitButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->ipLabel->AutoSize = true;
			this->ipLabel->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->ipLabel->Location = System::Drawing::Point(13, 13);
			this->ipLabel->Name = L"label1";
			this->ipLabel->Size = System::Drawing::Size(159, 13);
			this->ipLabel->TabIndex = 0;
			this->ipLabel->Text = L"Ingresa la dirección del servidor:";
			// 
			// textBox1
			// 
			this->ipTextBox->Font = (gcnew System::Drawing::Font(L"Century Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ipTextBox->Location = System::Drawing::Point(16, 30);
			this->ipTextBox->Name = L"textBox1";
			this->ipTextBox->Size = System::Drawing::Size(256, 27);
			this->ipTextBox->TabIndex = 1;

			// 
			// textBox2
			// 
			this->portTextBox->Font = (gcnew System::Drawing::Font(L"Century Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->portTextBox->Location = System::Drawing::Point(15, 80);
			this->portTextBox->Name = L"textBox2";
			this->portTextBox->Size = System::Drawing::Size(256, 27);
			this->portTextBox->TabIndex = 3;
			this->portTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::textBox2_TextChanged);
			// 
			// label2
			// 
			this->portLabel->AutoSize = true;
			this->portLabel->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->portLabel->Location = System::Drawing::Point(12, 63);
			this->portLabel->Name = L"label2";
			this->portLabel->Size = System::Drawing::Size(146, 13);
			this->portLabel->TabIndex = 2;
			this->portLabel->Text = L"Ingresa el puerto del servidor:";
			this->portLabel->Click += gcnew System::EventHandler(this, &Form1::label2_Click);
			// 
			// textBox3
			// 
			this->usernameTextBox->Font = (gcnew System::Drawing::Font(L"Century Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->usernameTextBox->Location = System::Drawing::Point(16, 129);
			this->usernameTextBox->Name = L"textBox3";
			this->usernameTextBox->Size = System::Drawing::Size(256, 27);
			this->usernameTextBox->TabIndex = 5;
			// 
			// label3
			// 
			this->usernameLabel->AutoSize = true;
			this->usernameLabel->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->usernameLabel->Location = System::Drawing::Point(13, 112);
			this->usernameLabel->Name = L"label3";
			this->usernameLabel->Size = System::Drawing::Size(106, 13);
			this->usernameLabel->TabIndex = 4;
			this->usernameLabel->Text = L"Ingresa tu username:";
			// 
			// button1
			// 
			this->submitButton->BackColor = System::Drawing::SystemColors::ControlDark;
			this->submitButton->FlatAppearance->BorderSize = 0;
			this->submitButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->submitButton->Font = (gcnew System::Drawing::Font(L"Century Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->submitButton->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->submitButton->Location = System::Drawing::Point(42, 173);
			this->submitButton->Name = L"button1";
			this->submitButton->Size = System::Drawing::Size(190, 30);
			this->submitButton->TabIndex = 6;
			this->submitButton->Text = L"Ingresar al server";
			this->submitButton->UseVisualStyleBackColor = false;
			this->submitButton->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->submitButton);
			this->Controls->Add(this->usernameTextBox);
			this->Controls->Add(this->usernameLabel);
			this->Controls->Add(this->portTextBox);
			this->Controls->Add(this->portLabel);
			this->Controls->Add(this->ipTextBox);
			this->Controls->Add(this->ipLabel);
			this->MaximumSize = System::Drawing::Size(300, 300);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"ChatSimple";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void textBox2_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void Form1_Load(System::Object^ sender, System::EventArgs^ e) {
	}
private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	nuevointento::WindowChat^ windowChat = gcnew nuevointento::WindowChat();
	windowChat->setIpAddress(this->ipTextBox->Text);
	windowChat->setPort(this->portTextBox->Text);
	windowChat->setUsername(this->usernameTextBox->Text);
	this->Visible = false;
	windowChat->ShowDialog();
	windowChat->run();
	//this->Visible = true;


}
};
}
