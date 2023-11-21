 #pragma once
#include "WindowChat.h"
namespace SimpleChat {

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
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ IpAddress;
	private: System::Windows::Forms::TextBox^ Port;



	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ Username;

	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ LoginServer;

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
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->IpAddress = (gcnew System::Windows::Forms::TextBox());
			this->Port = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->Username = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->LoginServer = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->label1->Location = System::Drawing::Point(13, 13);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(159, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Ingresa la dirección del servidor:";
			// 
			// IpAddress
			// 
			this->IpAddress->Font = (gcnew System::Drawing::Font(L"Century Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->IpAddress->Location = System::Drawing::Point(16, 30);
			this->IpAddress->Name = L"IpAddress";
			this->IpAddress->Size = System::Drawing::Size(256, 27);
			this->IpAddress->TabIndex = 1;
			// 
			// Port
			// 
			this->Port->Font = (gcnew System::Drawing::Font(L"Century Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Port->Location = System::Drawing::Point(15, 80);
			this->Port->Name = L"Port";
			this->Port->Size = System::Drawing::Size(256, 27);
			this->Port->TabIndex = 3;
			this->Port->TextChanged += gcnew System::EventHandler(this, &Form1::textBox2_TextChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->label2->Location = System::Drawing::Point(12, 63);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(146, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Ingresa el puerto del servidor:";
			this->label2->Click += gcnew System::EventHandler(this, &Form1::label2_Click);
			// 
			// Username
			// 
			this->Username->Font = (gcnew System::Drawing::Font(L"Century Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Username->Location = System::Drawing::Point(16, 129);
			this->Username->Name = L"Username";
			this->Username->Size = System::Drawing::Size(256, 27);
			this->Username->TabIndex = 5;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->label3->Location = System::Drawing::Point(13, 112);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(106, 13);
			this->label3->TabIndex = 4;
			this->label3->Text = L"Ingresa tu username:";
			// 
			// LoginServer
			// 
			this->LoginServer->BackColor = System::Drawing::SystemColors::ControlDark;
			this->LoginServer->FlatAppearance->BorderSize = 0;
			this->LoginServer->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->LoginServer->Font = (gcnew System::Drawing::Font(L"Century Gothic", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LoginServer->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->LoginServer->Location = System::Drawing::Point(42, 173);
			this->LoginServer->Name = L"LoginServer";
			this->LoginServer->Size = System::Drawing::Size(190, 30);
			this->LoginServer->TabIndex = 6;
			this->LoginServer->Text = L"Ingresar al server";
			this->LoginServer->UseVisualStyleBackColor = false;
			this->LoginServer->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoSize = true;
			this->BackColor = System::Drawing::SystemColors::ActiveCaptionText;
			this->ClientSize = System::Drawing::Size(284, 261);
			this->Controls->Add(this->LoginServer);
			this->Controls->Add(this->Username);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->Port);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->IpAddress);
			this->Controls->Add(this->label1);
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
	String^ ipAddress = IpAddress->Text;
	String^ port = Port->Text;
	String^ username = Username->Text;
	SimpleChat::WindowChat^ windowChat = gcnew SimpleChat::WindowChat();
	windowChat->IPAddressValue = ipAddress;
	windowChat->PortValue = port;
	windowChat->UsernameValue = username;
	this->Visible = false;
	windowChat->ShowDialog();
	this->Visible = true;

}
};
}
