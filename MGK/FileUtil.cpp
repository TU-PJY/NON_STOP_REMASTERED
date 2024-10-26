#include "FileUtil.h"
#include "Setting.h"

void FileUtil::Init(const char* FolderName, const char* FileName, DataSet List) {
	if (FileExist)
		return;

	std::string TempFileName = FileName;

	if (!USE_FILE_SECURITY)
		TempFileName += ".xml";

	std::filesystem::path FolderPath = FolderName;
	if (!std::filesystem::exists(FolderName)) {
		if (!std::filesystem::create_directory(FolderPath)) {
			std::cout << "Falied to find folder" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	FilePath = FolderPath / TempFileName;
	FilePathStr = FilePath.string();

	DataListBuffer = List;

	if (!LoadDataFile(FilePathStr.c_str()))
		SetupData();
	else {
		FileExist = true;
		std::cout << "File Util Opened file: " << FilePathStr << std::endl;
		CheckDataVersion();
	}
}

void FileUtil::UpdateData(const char* CategoryName, const char* DataName, float Value) {
	WriteDigitData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

void FileUtil::UpdateStringData(const char* CategoryName, const char* DataName, const char* Value) {
	WriteStringData(FindCategory(CategoryName), DataName, Value);
	UpdateDataFile();
}

float FileUtil::LoadData(const char* CategoryName, const char* DataName) {
	return GetDigitData(FindCategory(CategoryName), DataName);
}

const char* FileUtil::LoadStringData(const char* CategoryName, const char* DataName) {
	return GetStringData(FindCategory(CategoryName), DataName);
}

void FileUtil::ResetData() {
	Doc.Clear();
	FileExist = false;
	SetupData();
}

void FileUtil::Release() {
	Doc.Clear();
	FileExist = false;
}


//////////////////////////////// private
void FileUtil::SetupData() {
	if (!FileExist) {
		CreateDec(APPLICATION_VERSION);
		AddRoot("Data");
	}

	Root = FindRoot();

	for (auto const& D : DataListBuffer) {
		if(!FindCategory(D.CategoryName))
			AddCategory(D.CategoryName);

		if (!FindData(D.CategoryName, D.DataName)) {
			switch (D.DataType) {
			case DATA_TYPE_DIGIT:
				AddData(D.CategoryName, D.DataName, D.DigitValue);
				break;

			case DATA_TYPE_STRING:
				AddStringData(D.CategoryName, D.DataName, D.StringValue);
				break;
			}
		}
	}

	UpdateDataFile();

	if (!FileExist) {
		std::cout << "Created new data file: " << FilePathStr << std::endl;
		FileExist = true;
	}
}

void FileUtil::CheckDataVersion() {
	TiXmlNode* DeclNode = Doc.FirstChild();
	TiXmlDeclaration* Decl = DeclNode->ToDeclaration();

	if (Decl) {
		const char* Version = Decl->Version();
		if (std::stof(Version) < APPLICATION_VERSION)
			UpdateDataVersion(APPLICATION_VERSION);
	}
	else {
		std::cout << "Failed to find data version" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void FileUtil::UpdateDataVersion(float VersionValue) {
	TiXmlNode* DeclNode = Doc.FirstChild();
	TiXmlDeclaration* Decl = DeclNode->ToDeclaration();

	const char* Version = Decl->Version();
	const char* Encoding = Decl->Encoding();
	const char* StandAlone = Decl->Standalone();

	if (std::stof(Version) < VersionValue) {
		std::ostringstream OSS;
		OSS << std::fixed << std::setprecision(1) << VersionValue;
		std::string NewVersionStr = OSS.str();

		TiXmlDeclaration NewDecl(NewVersionStr.c_str(), Encoding ? Encoding : "", StandAlone ? StandAlone : "");

		Doc.ReplaceChild(DeclNode, NewDecl);

		SetupData();
	}
}

void FileUtil::CreateDec(float VersionValue) {
	std::ostringstream OSS;
	OSS << std::fixed << std::setprecision(1) << VersionValue;
	std::string NewVersionStr = OSS.str();
	Doc.LinkEndChild(new TiXmlDeclaration(NewVersionStr.c_str(), "", ""));
}

void FileUtil::AddRoot(const char* RootName) {
	Doc.LinkEndChild(new TiXmlElement(RootName));
}

void FileUtil::AddCategory(const char* CategoryName) {
	Root->LinkEndChild(new TiXmlElement(CategoryName));
}

void FileUtil::AddData(const char* CategoryName, const char* DataName, float Value) {
	FindCategory(CategoryName)->SetDoubleAttribute(DataName, Value);
}

void FileUtil::AddStringData(const char* CategoryName, const char* DataName, const char* Value) {
	FindCategory(CategoryName)->SetAttribute(DataName, Value);
}

void FileUtil::WriteDigitData(TiXmlElement* CategoryVar, const char* DataName, float Value) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		CategoryVar->SetDoubleAttribute(DataName, Value);
	else {
		std::cout << "Failed to update data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

void FileUtil::WriteStringData(TiXmlElement* CategoryVar, const char* DataName, const char* Value) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		CategoryVar->SetAttribute(DataName, Value);
	else {
		std::cout << "Failed to update string data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

int FileUtil::GetIntData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return std::stoi(DataValue);
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

float FileUtil::GetDigitData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return std::stof(DataValue);
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

const char* FileUtil::GetStringData(TiXmlElement* CategoryVar, const char* DataName) {
	const char* DataValue = CategoryVar->Attribute(DataName);
	if (DataValue)
		return DataValue;
	else {
		std::cout << "Failed to find data" << std::endl;
		exit(EXIT_FAILURE);
	}
}

TiXmlElement* FileUtil::FindRoot() {
	return Doc.RootElement();
}

TiXmlElement* FileUtil::FindCategory(const char* CategoryName) {
	return Root->FirstChildElement(CategoryName);
}

const char* FileUtil::FindData(const char* CategoryName, const char* DataName) {
	return FindCategory(CategoryName)->Attribute(DataName);
}

bool FileUtil::LoadDataFile(const char* FileName) {
	if (USE_FILE_SECURITY) {
		std::ifstream EncryptedFile(FileName, std::ios::binary);
		if (!EncryptedFile)
			return false;

		std::string EncryptedContent((std::istreambuf_iterator<char>(EncryptedFile)), std::istreambuf_iterator<char>());
		EncryptedFile.close();

		std::string DecryptedXML = Decrypt(EncryptedContent, AES_KEY, IV_KEY);
		Doc.Parse(DecryptedXML.c_str());

		if (Doc.Error()) {
			std::cout << "Failed to parse data file" << std::endl;
			exit(EXIT_FAILURE);
		}
		std::cout << "File Util Opened file: " << FilePathStr << std::endl;

		return true;
	}

	else
		return Doc.LoadFile(FileName);
}

void FileUtil::UpdateDataFile() {
	if (USE_FILE_SECURITY) {
		TiXmlPrinter Printer;
		Doc.Accept(&Printer);

		std::string XML_String = Printer.CStr();
		std::string EncryptedXML = Encrypt(XML_String, AES_KEY, IV_KEY);

		std::ofstream ExportFile(FilePathStr, std::ios::binary);
		ExportFile.write(EncryptedXML.c_str(), EncryptedXML.size());
		ExportFile.close();

		Doc.Clear();
		LoadDataFile(FilePathStr.c_str());
	}

	else
		Doc.SaveFile(FilePathStr.c_str());

	Root = FindRoot();
}

std::string FileUtil::Encrypt(const std::string& plainText, const byte key[], const byte iv[]) {
	std::string CipherText;

	try {
		CBC_Mode<AES>::Encryption Encryption;
		Encryption.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
		StringSource(plainText, true, new StreamTransformationFilter(Encryption, new StringSink(CipherText)));
	}
	catch (const Exception& E) {
		std::cerr << E.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return CipherText;
}

std::string FileUtil::Decrypt(const std::string& cipherText, const byte key[], const byte iv[]) {
	std::string PlainText;

	try {
		CBC_Mode<AES>::Decryption Decryption;
		Decryption.SetKeyWithIV(key, AES::DEFAULT_KEYLENGTH, iv);
		StringSource(cipherText, true, new StreamTransformationFilter(Decryption, new StringSink(PlainText)));
	}
	catch (const Exception& E) {
		std::cerr << E.what() << std::endl;
		exit(EXIT_FAILURE);
	}

	return PlainText;
}