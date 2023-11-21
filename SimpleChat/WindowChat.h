#pragma once

namespace SimpleChat {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de WindowChat
	/// </summary>
	public ref class WindowChat : public System::Windows::Forms::Form
	{
	public:
		WindowChat(void)
		{
			InitializeComponent();
			//
			//TODO: agregar código de constructor aquí
			//
		}
		property String^ IPAddressValue {
			String^ get() { return ipAddressValue; }
			void set(String^ value) { ipAddressValue = value; }
		}

		property String^ PortValue {
			String^ get() { return portValue; }
			void set(String^ value) { portValue = value; }
		}

		property String^ UsernameValue {
			String^ get() { return usernameValue; }
			void set(String^ value) { usernameValue = value; }
		}

	protected:
		/// <summary>
		/// Limpiar los recursos que se estén usando.
		/// </summary>
		~WindowChat()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^ panel1;
	private: System::Windows::Forms::Panel^ panel3;
	private: System::Windows::Forms::TextBox^ MessageTextBox;

	private: System::Windows::Forms::Panel^ panel2;
	private: System::Windows::Forms::TextBox^ UsersList;

	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ AllMessages;

	private: System::Windows::Forms::Button^ BtnSend;
	private: String^ ipAddressValue;
	private: String^ portValue;
	private: String^ usernameValue;

	protected:

	private:
		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Método necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este método con el editor de código.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(WindowChat::typeid));
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->AllMessages = (gcnew System::Windows::Forms::TextBox());
			this->panel3 = (gcnew System::Windows::Forms::Panel());
			this->BtnSend = (gcnew System::Windows::Forms::Button());
			this->MessageTextBox = (gcnew System::Windows::Forms::TextBox());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->UsersList = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->panel1->SuspendLayout();
			this->panel3->SuspendLayout();
			this->panel2->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->BackColor = System::Drawing::SystemColors::ControlDark;
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->AllMessages);
			this->panel1->Controls->Add(this->panel3);
			this->panel1->Controls->Add(this->panel2);
			this->panel1->Location = System::Drawing::Point(-1, 0);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(963, 417);
			this->panel1->TabIndex = 0;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Yu Gothic UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->label2->Location = System::Drawing::Point(220, 9);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(196, 21);
			this->label2->TabIndex = 3;
			this->label2->Text = L"Bienvenid@s a ChatSimple";
			// 
			// AllMessages
			// 
			this->AllMessages->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->AllMessages->Font = (gcnew System::Drawing::Font(L"Yu Gothic UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->AllMessages->Location = System::Drawing::Point(215, 39);
			this->AllMessages->Multiline = true;
			this->AllMessages->Name = L"AllMessages";
			this->AllMessages->ReadOnly = true;
			this->AllMessages->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->AllMessages->Size = System::Drawing::Size(735, 315);
			this->AllMessages->TabIndex = 2;
			// 
			// panel3
			// 
			this->panel3->BackColor = System::Drawing::SystemColors::GrayText;
			this->panel3->Controls->Add(this->BtnSend);
			this->panel3->Controls->Add(this->MessageTextBox);
			this->panel3->Location = System::Drawing::Point(215, 353);
			this->panel3->Name = L"panel3";
			this->panel3->Size = System::Drawing::Size(735, 61);
			this->panel3->TabIndex = 1;
			// 
			// BtnSend
			// 
			this->BtnSend->BackColor = System::Drawing::Color::Transparent;
			this->BtnSend->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->BtnSend->Cursor = System::Windows::Forms::Cursors::Hand;
			this->BtnSend->FlatAppearance->BorderColor = System::Drawing::Color::Black;
			this->BtnSend->FlatAppearance->BorderSize = 0;
			this->BtnSend->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->BtnSend->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject(L"BtnSend.Image")));
			this->BtnSend->Location = System::Drawing::Point(670, 8);
			this->BtnSend->Name = L"BtnSend";
			this->BtnSend->Size = System::Drawing::Size(50, 50);
			this->BtnSend->TabIndex = 1;
			this->BtnSend->UseVisualStyleBackColor = false;
			this->BtnSend->Click += gcnew System::EventHandler(this, &WindowChat::button1_Click);
			// 
			// MessageTextBox
			// 
			this->MessageTextBox->Font = (gcnew System::Drawing::Font(L"Yu Gothic UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->MessageTextBox->Location = System::Drawing::Point(19, 17);
			this->MessageTextBox->Multiline = true;
			this->MessageTextBox->Name = L"MessageTextBox";
			this->MessageTextBox->Size = System::Drawing::Size(645, 33);
			this->MessageTextBox->TabIndex = 0;
			// 
			// panel2
			// 
			this->panel2->BackColor = System::Drawing::SystemColors::ControlText;
			this->panel2->Controls->Add(this->UsersList);
			this->panel2->Controls->Add(this->label1);
			this->panel2->Location = System::Drawing::Point(3, 0);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(211, 414);
			this->panel2->TabIndex = 0;
			// 
			// UsersList
			// 
			this->UsersList->Enabled = false;
			this->UsersList->Font = (gcnew System::Drawing::Font(L"Yu Gothic UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->UsersList->Location = System::Drawing::Point(14, 39);
			this->UsersList->Multiline = true;
			this->UsersList->Name = L"UsersList";
			this->UsersList->ReadOnly = true;
			this->UsersList->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->UsersList->Size = System::Drawing::Size(184, 315);
			this->UsersList->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Yu Gothic UI", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::SystemColors::ControlLightLight;
			this->label1->Location = System::Drawing::Point(10, 9);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(160, 21);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Participantes del chat:";
			// 
			// WindowChat
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(961, 415);
			this->Controls->Add(this->panel1);
			this->MaximumSize = System::Drawing::Size(977, 454);
			this->Name = L"WindowChat";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"WindowChat";
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panel3->ResumeLayout(false);
			this->panel3->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		String^Menssage = MessageTextBox->Text + "\r\n";
		AllMessages->Text = AllMessages->Text + Menssage;
		MessageTextBox->Text = "";
	}
};
}
