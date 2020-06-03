#include <iostream> // I/O stream : C++���� ���� stdio.h�� �ش�
#include <fstream> // ���� �����
#include <string> // string Ÿ�� ���
#include <vector> // ���� Ÿ�� ���, string Ÿ���� �迭 ��� ��������
#include <sstream> // ���ڿ����� �ʿ��� ������ ������ ���
#include <cstdlib> // int�� ���밪 �Լ� abs�� Ÿ�� ��ȯ �Լ� atoi ����Ϸ��� ����
#include <time.h>
#include <cctype>
#include <windows.h>
#include <conio.h>

using namespace std;
#pragma warning(disable:4996) // _s �ø��� �Լ��� ����ϰԲ� ����� �ȵǼ� �̸� ������.
int db_check=0;
int reopen = 0;
/*
tip) ����Ҷ� �ǵ��� printf�� ���� ���� :
'cout << endl;' ������ printf ���� �޸𸮸� �ξ� ���� �Ա� �����̴�.
����ȭ�� ���ؼ� cout ���� printf�� ���� ���� ����.

�׷����� ���� cout�� ���� ���� : �������̰� ���� ���ϴ�.

*/


class DailyChart { // ���� ���� ó���� ��� �� Ŭ�������� ó���� ����. (����ȣ)
private:
	int num = 0; // ������ȣ
	int year = 0; // ��
	int month = 0; // ��
	int day = 0; //��
	string start_time; // ���� �ð�
	string end_time; // ���� �ð�
	int btw_time = 0; // �ð� ����
	string cartegory; // ī�װ�
	string detail_chart; // ���λ���
public:
	int Daily_DB_Check(string *, int*);
	void Daily_DB_Read(vector<string>*);   //  �����ͷ� �Ѱ� �޾ƾ� ������ All_Database�� ������ ��� ������ �ȴ�.
	void Daily_DB_Search(vector<string>*, string, int); // �ش� ���� �˻� �Լ�
	void Schedule_make(vector<string>*, string); // ���� �Է� �Լ�
	void Schedule_modify(vector<string>*, string); // ���� ���� �Լ�
	void Schedule_delete(vector<string>*, string); // ���� ���� �Լ�
	int CheckP_ID(vector<string>*, string); // ������ȣ Ȯ��
	void Daily_statistics(vector<string>*, string); // �ϰ� ���
	void Monthly_statistics(vector<string>*, string); // ���� ���
};

vector<string> All_Database; // ������ ����� ��ü DB ���� ���� ����

void menu_1();  // �޴�1�� �����Է�/����/���� ���̴�
void menu_2();  // �޴�2�� �ϰ���� ���̴�
void menu_3();  // �޴�3�� ������� ���̴�
void menu_4();  // �޴�4�� ����� ���̴�
void menu_5();  // �޴�5�� �������̴�


int DailyChart::Daily_DB_Check(string *err_text, int* a) { // DB üũ �Լ� (����ȣ)

	ifstream CheckFile("DB.txt");


	int count = 0, i = 0, length = 0, check_id = 0;
	int count2 = 0; //�� ���ڰ� ���� ��츦 ��� ���� ī���� ����

	// �� �׽�Ʈ�� ���� ���� ����
	string num_err;
	string year_err;
	string month_err;
	string day_err;
	string start_time_err;
	string end_time_err;
	string btw_time_err;
	string cartegory_err;
	string detail_chart_err;

	int check_tmp = 0;

	stringstream sstream_test;
	

	if (CheckFile.is_open()) { // DB.txt ������ ������

		//��¥ �ش� ���α׷����� �ʿ��� DB���� ���� Ȯ��


		while (!CheckFile.eof()) { // DB�� ����������

			string tmp;
			std::getline(CheckFile, tmp); // ���پ� �о��.

			sstream_test.clear();
			sstream_test.str(tmp);
			
			// �� ���� �ʱ�ȭ
			num_err.clear();
			year_err.clear();
			month_err.clear();
			day_err.clear();
			start_time.clear();
			end_time_err.clear();
			btw_time_err.clear();
			cartegory_err.clear();
			detail_chart_err.clear();

			sstream_test >> num_err >> year_err >> month_err >> day_err >> start_time_err >> end_time_err >> btw_time_err >>
				cartegory_err >> detail_chart_err; // Ŭ���� ���� ����

			count = 0;
			i = 0;
			check_id = atoi(num_err.c_str());

			length = tmp.length();

			if (length == 0){ // ���̰� 0�̶� ���� �����ų� �߰��� ���Ͱ� �ִ� ���̴�.
				if (CheckFile.eof()){ // ���� ������ ����������
					CheckFile.close();
					break; // ����
				}
				else { // ������ �������� ������ (���Ͷ����� �ڰ� �̾����ִٸ�)
					// �������
					CheckFile.close();
					*a = check_id; // �� ��° ������ȣ���� ���� ������ ��ȯ
					*err_text = "�Ӽ�";
					return 1;
				}


			}

			while (i <= length) {
				if (tmp[i] == '\t') { //1. ���� ������ Ȯ���Ͽ� �Ӽ� ������ Ȯ��.

					count++;
				}

				i++;

			}

			if (count == 8) { // �Ӽ��� ������ 9���� ��ġ��. (\t�� �� �׸� ���̿��� �����ϹǷ� 8���� ����.)

			}
			else { // ������ ��ġ���� ����. �ϴ� �츮�� DB�� �ƴ�.
				CheckFile.close();
				*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
				*err_text = "�Ӽ�";
				return 1;
			}

			/*
			@ ���� �� ���ں� ���� �κ��� Ȯ��
			 point ) ������ ���� ������ �Ǿ��ִٴ� ���� Ȯ�������� 
			 �ش� ���� (������ȣ)\t(�⵵)\t(��)\t(��)\t(���۽ð�)\t(�����ð�)\t(�ð���)\t(ī�װ�)\t(���λ���) ���� �ۼ��Ǿ��ִ�.
			 �� ���� �������

			//\t(�⵵)\t(��)\t(��)\t(���۽ð�)\t(�����ð�)\t(�ð���)\t(ī�װ�)\t(���λ���)
			//(������ȣ)\t\t(��)\t(��)\t(���۽ð�)\t(�����ð�)\t(�ð���)\t(ī�װ�)\t(���λ���)
			//(������ȣ)\t(�⵵)\t\t(��)\t(���۽ð�)\t(�����ð�)\t(�ð���)\t(ī�װ�)\t(���λ���)
			//(������ȣ)\t(�⵵)\t(��)\t\t(���۽ð�)\t(�����ð�)\t(�ð���)\t(ī�װ�)\t(���λ���)
			//(������ȣ)\t(�⵵)\t(��)\t(��)\t\t(�����ð�)\t(�ð���)\t(ī�װ�)\t(���λ���)
			//(������ȣ)\t(�⵵)\t(��)\t(��)\t(���۽ð�)\t\t(�ð���)\t(ī�װ�)\t(���λ���)
			//(������ȣ)\t(�⵵)\t(��)\t(��)\t(���۽ð�)\t(�����ð�)\t\t(ī�װ�)\t(���λ���)
			//(������ȣ)\t(�⵵)\t(��)\t(��)\t(���۽ð�)\t(�����ð�)\t(�ð���)\t\t(���λ���)

			���� ������ �� �� �ִ�.

			��, ó������ ���� �������� �ι� ��ġ�� ������ �� ��� �Ǵ� ���̴�.
			���⼭ ������ ������ȣ�� �����ϴ� ���̴�.
			*/

			count2 = 0;

			for (i = 0; i < length - 1; i++)
			{
				if (tmp[0] == '\t') // ������ȣ ���ڰ� ���� ��
				{
					CheckFile.close();
					*a = NULL; // ������ȣ�� ������ NULL ��ȯ
					*err_text = "������ȣ";
					return 1;
				}

				if (tmp[i] == '\t')
					count2++; // �� ���� ī��Ʈ

				if (tmp[i] == '\t' && tmp[i + 1] == '\t') // �ٷ� �ڿ� �������� ������?
				{
					if (count2 == 1)
					{
						CheckFile.close();
						*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
						*err_text = "�⵵";
						return 1;
					}
					else if (count2 == 2)
					{
						CheckFile.close();
						*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
						*err_text = "��";
						return 1;
					}
					else if (count2 == 3)
					{
						CheckFile.close();
						*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
						*err_text = "��";
						return 1;
					}
					else if (count2 == 4)
					{
						CheckFile.close();
						*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
						*err_text = "���۽ð�";
						return 1;
					}
					else if (count2 == 5)
					{
						CheckFile.close();
						*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
						*err_text = "�����ð�";
						return 1;
					}
					else if (count2 == 6)
					{
						CheckFile.close();
						*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
						*err_text = "�ð���";
						return 1;
					}
					else if (count2 == 7)
					{
						CheckFile.close();
						*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
						*err_text = "ī�װ�";
						return 1;
					}
				}
			}


			/*
			(������ȣ)\t(�⵵)\t(��)\t(��)\t(���۽ð�)\t(�����ð�)\t(�ð���)\t(ī�װ�)\t <- ���λ��׸� ���� ���� �Ʒ� �������� �� �ɸ����Ƿ� ����.
			*/
			
			
			//2. �� �Ӽ��� ������ Ȯ��

			// ������ȣ Ȯ��
			if (atoi(num_err.c_str()) < 0){
				CheckFile.close();
				*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
				*err_text = "������ȣ";
				return 1;
			}

			// ���� Ȯ��
			if (atoi(year_err.c_str()) < 2000 || atoi(year_err.c_str()) > 2999){
				if (check_id == 0) { // ó�� DB.txt ������ �����Ҷ� ù ������ȣ�� 0�̹Ƿ� �̴� ������.

				}
				else {
					CheckFile.close();
					*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
					*err_text = "����";
					return 1;
				}

			}

			// �� Ȯ��
			if (atoi(month_err.c_str()) < 1 || atoi(month_err.c_str()) > 12){
				if (check_id == 0) { // ó�� DB.txt ������ �����Ҷ� ù ������ȣ�� 0�̹Ƿ� �̴� ������.

				}
				else {
					CheckFile.close();
					*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
					*err_text = "��";
					return 1;
				}
			}

			// �� Ȯ��
			if (atoi(day_err.c_str()) < 1 || atoi(day_err.c_str()) > 31){
				if (check_id == 0){ // ó�� DB.txt ������ �����Ҷ� ù ������ȣ�� 0�̹Ƿ� �̴� ������.

				}
				else {
					CheckFile.close();
					*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
					*err_text = "��";
					return 1;
				}
			}

			//�ð� Ȯ��

			if (atoi(start_time_err.c_str()) < 0 || atoi(start_time_err.c_str()) > 2400){
				CheckFile.close();
				*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
				*err_text = "���۽ð�";
				return 1;
			}

			if (atoi(end_time_err.c_str()) < 0 || atoi(end_time_err.c_str()) > 2400){
				CheckFile.close();
				*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
				*err_text = "�����ð�";
				return 1;
			}

			// �ð��� Ȯ��

			if (atoi(btw_time_err.c_str()) < 0 || atoi(btw_time_err.c_str()) > 1440){
				CheckFile.close();
				*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
				*err_text = "�ð���";
				return 1;
			}



			//ī�װ� Ȯ��
			if (cartegory_err == "����" || cartegory_err == "�Ļ�" || cartegory_err == "����" || cartegory_err == "����" || cartegory_err == "�" || cartegory_err == "���" || cartegory_err == "�Ƹ�����Ʈ" || cartegory_err == "��Ÿ"){

			}
			else if (check_id == 0) { // ó�� DB.txt ������ �����Ҷ� ù ������ȣ�� 0�̹Ƿ� �̴� ������.

			}
			else {
				CheckFile.close();
				*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
				*err_text = "ī�װ�";
				return 1;
			}

			// ���γ��� Ȯ��
			int n_byte = strlen(detail_chart_err.c_str());
			if (n_byte <= 0 || n_byte > 100){
				CheckFile.close();
				*a = check_id; // �� ��° �ٿ��� ���� ������ ��ȯ
				*err_text = "���γ���";
				return 1;

			}

		}




	}
	else { // DB.txt ������ ������ ����

		CheckFile.close();

		ofstream GenerateFile;
		GenerateFile.open("DB.txt"); // ���� ����
		GenerateFile.close(); // ���� �ݱ�


		// ���� ����

		ofstream WriteFile("DB.txt", ios::app);

		string str = "0\t0\t0\t0\t0\t0\t0\t0\t0\n";

		WriteFile << str;

		WriteFile.close();


		return 0;
	}
	return 0;
}

void DailyChart::Daily_DB_Read(vector<string>* db) { // DB �о�ͼ� ��ȯ�ϴ� �Լ� (����ȣ)

	ifstream readfile; // �о���� ���� ����



	readfile.open("DB.txt");


	if (readfile.is_open()) {
		while (!readfile.eof()) {
			string str;
			std::getline(readfile, str); // DB.txt ������ ���پ� �о��


			(*db).push_back(str); // �о�� ������ db �������ڿ� �������� ������.

		}
		
	}



	readfile.close();

}


void DailyChart::Daily_DB_Search(vector<string>* db, string date_data, int option) { // DB���� ã�� ���� ��ȯ�ϴ� �Լ� (����ȣ)

	/* ���� ����

	vector<string> db : DB ���� ������ ����.

	string date_data : ó���� �Է� ���� ¥���� 2020-03-20 �̷� string ���·� �޾���.

	*/

	int D_year = 0, D_month = 0, D_day = 0; // date_data���� �Է� ���� ������� �Ҵ��� ����

	All_Database.clear(); //
	db->clear();
	Daily_DB_Read(&All_Database); // ���� ��ü DB ����
	

	int index_count = 0;

	vector<string> DB_test; // ��ġ �ȿ��� ����� �Լ�

	/* ���� ����

	�� �Լ��� �⺻������ 2020-03-20 ���� ���·� ������� ���а����� ���ڷ�
	string�� �޾Ƶ鿴�ٴ� ������ ���۵Ǿ���.

	���ܷ� ���� ���� ���� �� ���߿� ������ ����.

	*/

	stringstream stream_test;
	stream_test.str(date_data);

	stream_test >> D_year >> D_month >> D_day; // ����� ����
	stream_test.clear();


	/* �� ���� ���й�

	stringstream�� ���� int ������ year, month, day�� ������� int���� �������� ������
	2020-03-20 �� ���  2020, -03, -20���� �޾Ƶ鿩�� ���밪 ����.
	�׷�, 2020�� 3�� 20�Ϸ� �з��� �� ����.

	2020_03_20 �̳� '2020 03 20' �� ��� ���б�ȣ�� �ƴ� ��÷�ڳ� �����̶�
	�˾Ƽ� 2020�� 3�� 20�Ϸ� �з���

	���� �Է� �ܰ迡�� ��, ��, ���� ���ĸ� ����� �Է� ������
	�ƹ����Գ��� �˾Ƽ� �з��ȴ�.

	####
	-> �̷� �з����� ���� : 20200203 ���� ������ ���� �Է� ������ �з��� �ȵ�.
							�̷� ��� �����ڸ� ������༭ �ٽ� ��������ϴ� ����ο� �ڵ带 ������������ �ϴ� ����

							+) 2020.07.20 �� ���, double ������ �ν��� �Ǿ �ƿ� �ȵ��ư���...
							�� ����� ����ó���� �� �����غ���

	*/

	D_year = abs(D_year);
	D_month = abs(D_month);
	D_day = abs(D_day);

	int size = (All_Database).size();  //db ũ�� ����� �Ϸ��� �����

	if (option == 1) {
		printf("********************%d�� %d�� %d���� �ϰ� ***********************\n\n", D_year, D_month, D_day);
		printf("������ȣ\t���۽ð�\t�����ð�\tȰ��\t\t���γ���\n\n");
	}

	for (int i = 0; i < All_Database.size() - 1; i++) { // db ũ�� ��ŭ ��� ���� ������.

		// �� �ึ�� ��ġ�ϴ� ��¥�� ���� ������ �ִ��� ����!


		/*
		���� DB�� db[0] = "1\t2020\t03\t20\t0600\t0900\t180\t�Ļ�\t��ħ�Ļ�"
		�̷� ������ ������ �Ǿ��ִ�.

		���̺� ������ ������� int, int, int, int, string, string, int, string, string �̹Ƿ�

		�� ������� sstream�� �̿��� ���ڸ� �޴´�.

		���⼭ vector�� string ���̱⿡ db[i]�� ��� ������ �� string ���� ���� �������.
		*/


		stream_test.clear(); // sstream�� �ѹ� ���� ���� �ٽ� �ʱ�ȭ ���Ѿ� �� �� �ִ�.

		stream_test.str((All_Database)[i]);

		stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
			cartegory >> detail_chart; // Ŭ���� ���� ����


		if (D_year == year && D_month == month) {

			if (option && D_day == day) { // ��¥ ������ ��ġ�ϸ�
				
				(*db).push_back((All_Database)[i]); // �о�� ������ db �������ڿ� �������� ������.	
				DB_test.push_back((All_Database)[i]);
				index_count++;
			}
			else if(option == 2 && D_day == day)
			{
				(*db).push_back((All_Database)[i]); // �о�� ������ db �������ڿ� �������� ������.	
				DB_test.push_back((All_Database)[i]);
				index_count++;
			}
			else if (!option) { // ����� �´��� Ȯ���ؼ� (���� ���)
				(*db).push_back((All_Database)[i]); // �о�� ������ db �������ڿ� �������� ������.
				DB_test.push_back((All_Database)[i]);
				index_count++;
			}


		}



	}

	//�ð�������� ����� �ϰ� ���(������)


	int num_j = 0; // ������ȣ
	int year_j = 0; // ��
	int month_j = 0; // ��
	int day_j = 0; //��
	int start_time_i;
	int start_time_j; // ���� �ð�
	int tmp;
	int k=0, count_k=0;
	string k_test;

	
	if (index_count == 0){
		printf("***************************************************************\n");
	}
	else if (option == 1) {



		for (size_t i = 0; i < DB_test.size() - 1; i++) { // �ϰ� ���� ���� ���� ���
			stream_test.clear();
			string temp;
			tmp = i;
			stream_test.str(DB_test[i]);
			stream_test >> num >> year >> month >> day >> start_time_i; // Ŭ���� ���� ����


			for (size_t j = i + 1; j < DB_test.size(); j++) {

				stream_test.clear();
				stream_test.str(DB_test[j]);

				stream_test >> num_j >> year_j >> month_j >> day_j >> start_time_j; // Ŭ���� ���� ����

				if (start_time_i > start_time_j) {

					temp = DB_test[i];
					DB_test[i] = DB_test[j];
					DB_test[j] = temp;

					stream_test.clear(); // �ڸ��� �ٲ����� �ٽ� �ѹ� ������
					stream_test.str(DB_test[i]);
					stream_test >> num >> year >> month >> day >> start_time_i; // Ŭ���� ���� ����

				}

			}

		}


		//���⿡ ���
		for (size_t i = 0; i < DB_test.size(); i++) {
			k_test = DB_test[i];

			stream_test.clear();
			stream_test.str(DB_test[i]);
			stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
				cartegory >> detail_chart; // Ŭ���� ���� ����		

			
			count_k = 0;

			//#### ���λ��� �����̽��ٱ��� �����.
			for (k = 0; k < k_test.length() ; k++)
			{
				if (k_test[k] == '\t')
				{
					count_k++;
				}

				if (count_k == 8) // ���� �Ӽ��� ���λ����϶�
				{
					detail_chart = k_test.substr(k+1, k_test.length() - k -1 ); // ���� �ִ� �ڸ� �������� ������ ����
					break;
				}
			}

			if (cartegory == "�Ƹ�����Ʈ"){
				cout << num << "\t\t" << start_time.substr(0, 2) + ":" + start_time.substr(start_time.length() - 2, 2) << "\t\t" << end_time.substr(0, 2) + ":" + end_time.substr(start_time.length() - 2, 2) << "\t\t" << cartegory << "\t" << detail_chart << '\n' << endl; // �ش� ���� ���� ���
			}
			else {
				cout << num << "\t\t" << start_time.substr(0, 2) + ":" + start_time.substr(start_time.length() - 2, 2) << "\t\t" << end_time.substr(0, 2) + ":" + end_time.substr(start_time.length() - 2, 2) << "\t\t" << cartegory << "\t\t" << detail_chart << '\n' << endl; // �ش� ���� ���� ���
			}

		}

		printf("***************************************************************\n");
	}

}


int ReturnBtwTime(int start_minute, int end_minute, int start_hour, int end_hour) { // �ð��� �����ִ� �Լ� (����ȣ)


	int Time;

	// �д����� ȯ���ؼ� ����

	if (end_minute - start_minute < 0) { // �г��� ���� �� ������ ���ö� ; ��) 0149 ~ 0239 �� ���

		Time = ((end_hour - 1) - start_hour) * 60
			+ (end_minute + 60) - start_minute;
	}
	else { // ������ ����� �� �ִ� ���

		Time = (end_hour - start_hour) * 60 + end_minute - start_minute;
	}



	return Time;
};



int CheckDetail(string* detail) { // ���λ��� ���� üũ �Լ� (����ȣ)

	/* ���� �����̽��� ��ȯ�����ְ�, 100byte�� �ѱ��� �ʾҳ� üũ*/

	// ǥ��ȭ

	for (int i = 0; i < detail->size(); i++) {

		if ((*detail)[i] == '\t') { // ��('\t') ����(' ') ó��

			(*detail)[i] = ' ';
		}
	}

	
	int n_byte = strlen(detail->c_str()); // byte ��ȯ


	if (n_byte <= 100 && n_byte >= 1) {
		return 1;
	}
	else if (n_byte == 0){
		printf("�ݵ�� 1�� �̻� ����մϴ�.\n");
		return 0;
	}
	else {
		printf("[%d]byte�� �Է��� �� �ִ� ���ڿ��� �ʰ��߽��ϴ�. 100byte���� �Է� �����մϴ�.\n", n_byte);
		return 0;
	}

};




//��¥�� ����ó�� �Լ���(���Ʊ�)  
int onjum(char a[]); //���й��� ���� �Է½� �������� �������� Ȯ���ϴ� �Լ� 
int slash(char a[]); //���й��� ������ �Է½� �������� �������� Ȯ���ϴ� �Լ�
int space(char a[]); //���й��� ���� �Է½� �������� �������� Ȯ���ϴ� �Լ�
int minus1(char a[]);//���й��� ���̳ʽ� �Է½� ���̳ʽ��� �������� Ȯ���ϴ� �Լ�
int year1(char a[]); // ���й��� ���°�� �⵵�� �迭�� �־��ִ� �Լ�
int month1(char a[]); // ���й��� ���°�� ���� �迭�� �־��ִ� �Լ�
int date1(char a[]); // ���й��� ���°�� ���� �迭�� �־��ִ� �Լ�
int date2(char a[]); // ���й��� ���°�� ���� �迭�� �־��ִ� �Լ�
int month2(char a[]); // ���й��� ���°�� ���� �迭�� �־��ִ� �Լ�
int zerogu1(char a[]); //���๮�� '0'�� �ΰ��̻� �ִ��� Ȯ���ϴ� �Լ�
int zerogu3(char a[]); //���๮�� '0'�� �ΰ��̻� �ִ��� Ȯ���ϴ� �Լ�
int zerogu5(char a[]); //���๮�� '0'�� �ΰ��̻� �ִ��� Ȯ���ϴ� �Լ�
int zerogu6(char a[]); //���๮�� '0'�� �ΰ��̻� �ִ��� Ȯ���ϴ� �Լ�
int sign(char a[]); //�߸��� ���ڰ� �ִ��� Ȯ���ϴ� �Լ�
int signmiss(char a[]); // ���й��ڰ� �������� �������� Ȯ���ϴ� �Լ�
int zerogu4(char a[]); //�⵵�� ���� '0'
int last(char a[]); // �������� ���й��� 

//(���Ʊ�) cf) (��) insert_date �Լ�
int DateCheck(string* search_date) { // ��¥ ���� üũ �Լ� : �����̸� 0���� ������ 1����



	//����, - , / , . ���� ���а���
	// 2000- 2999 �⵵
	// 01 ,1  001(x)

	 //���� " -/."�� ���й��ڰ� ���ƴ϶�� ���� 

	int o1, o2, o3, o4, z1,z2,z3,z4, si, si2, z5,l;
	o1 = slash((char*)search_date->c_str());
	o2 = onjum((char*)search_date->c_str());
	o3 = space((char*)search_date->c_str());
	o4 = minus1((char*)search_date->c_str());
	z1 = zerogu1((char*)search_date->c_str()); // o o ��
	z2 = zerogu3((char*)search_date->c_str());// o o ��
	z3 = zerogu5((char*)search_date->c_str());// o x ��
	z4 = zerogu6((char*)search_date->c_str());// o x ��
	si = sign((char*)search_date->c_str());
	si2 = signmiss((char*)search_date->c_str());
	z5 = zerogu4((char*)search_date->c_str());
	l = last((char*)search_date->c_str());

	if (si == 2) {
		printf("�߸��� ���ڰ� �ֽ��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
		return 1;
	}
	if ((*search_date)[0] == '.' || (*search_date)[0] == '/' || (*search_date)[0] == ' ' || (*search_date)[0] == '-') {
		printf("���Ŀ� ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
		return 1;
	}
	if (si2 == 2) {
		printf("���ڿ��� �ѹ��� �� �� �̻� �Է��߽��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
		return 1;
	}
	if (l == 2) {
		printf("���Ŀ� ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
		return 1;
	}

	char* oper = strchr((char*)search_date->c_str(), '.');
	char* oper1 = strchr((char*)search_date->c_str(), '/');
	char* oper2 = strchr((char*)search_date->c_str(), ' ');
	char* oper3 = strchr((char*)search_date->c_str(), '-'); //��͵� ã�Ƽ� ��ġ��ȯ
	char* num = strtok((char*)search_date->c_str(), " -/.");

	int nums[100];
	int i = 0; ; int j = 0;
	char nums2[100];

	if (oper != NULL || oper1 != NULL || oper2 != NULL || oper3 != NULL) { // ���й��ڰ� �ִ��� ������ ���� ����
		//���й��� ���� ���� ����
		while (num != NULL) { //�迭�� ���ڵ� ����
			nums[i++] = atoi(num);
			num = strtok(NULL, " -/.");
		}//  �⵵���� i�� 3��ȯ 4������ �߸���
		if (i > 3) {
			printf("���ڿ��� �ùٸ��� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
			return 1;
		}
		else {
			if (i == 3) {
				if (o1 == 1 || o2 == 1 || o3 == 1 || o4 == 1) {
					if (z5 == 2) {
						printf("�⵵ ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
						return 1;
					}
					if (nums[0] >= 2000 && nums[0] <= 2999) {
						
						if (nums[1] == 1 || nums[1] == 3 || nums[1] == 5 || nums[1] == 7
							|| nums[1] == 8 || nums[1] == 10 || nums[1] == 12) {
							if (z1 == 2) {
								printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
								return 1;
							}
							if (nums[2] >= 1 && nums[2] <= 31) {
								//printf("���߿� �����ͺ��̽��� ��¥ ������ �ֱ�\n");
								if (z2 == 2) {
									printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
									return 1;
								}
								string y = to_string(nums[0]);
								string m = to_string(nums[1]);
								string d = to_string(nums[2]);
								*search_date = y + '-' + m + '-' + d;
								
								return 0;
							}
							else {
								printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
								return 1;
							}
						}
						else if (nums[1] == 4 || nums[1] == 6 || nums[1] == 9 || nums[1] == 11) {
							if (nums[2] >= 1 && nums[2] <= 30) {
								if (z1 == 2) {
									printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
									return 1;
								}
								//printf("���߿� �����ͺ��̽��� ��¥ ������ �ֱ�2\n");
								if (z2 == 2) {
									printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
									return 1;
								}
								string y = to_string(nums[0]);
								string m = to_string(nums[1]);
								string d = to_string(nums[2]);
								*search_date = y + '-' + m + '-' + d;
								return 0;
							}
							else {
								printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
								return 1;
							}
						}
						else if (nums[1] == 2) {
							if (z1 == 2) {
								printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
								return 1;
							}
							int k;
							k = nums[0] % 4;
							if (nums[0] % 4 == 0 && nums[0] % 100 != 0 || nums[0] % 400 == 0) {
								if (nums[2] >= 1 && nums[2] <= 29) {
									//printf("���߿� �����ͺ��̽��� ��¥�ֱ�3\n");
									if (z2 == 2) {
										printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
										return 1;
									}
									string y = to_string(nums[0]);
									string m = to_string(nums[1]);
									string d = to_string(nums[2]);
									*search_date = y + '-' + m + '-' + d;
									return 0;
								}
								else {
									printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
									return 1;
								}
							}
							else {
								if (nums[2] >= 1 && nums[2] <= 28) {
									if (z2 == 2) {
										printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
										return 1;
									}
									//printf("���߿� �����ͺ��̽��� ��¥�ֱ�4\n");
									string y = to_string(nums[0]);
									string m = to_string(nums[1]);
									string d = to_string(nums[2]);
									*search_date = y + '-' + m + '-' + d;
									return 0;
								}
								else {
									printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
									return 1;
								}
							}
						}
						else {
							printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
							return 1;
						}
					}
					else {
						printf("�⵵ ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
						return 1;
					}
				}
				else {
					printf("���й��ڰ� �������� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
					return 1;
				}
			}//�⵵ ���� ���Է��Ѱ�
			else if (i == 2) {
				//�⵵�� �Է����� �ʾұ� ������ �ڵ����� ��ǻ�� �⵵ �������ֱ�
				time_t base_time = time(NULL);
				struct tm* newtime = localtime(&base_time);
				int t = newtime->tm_year + 1900; //���ó⵵���� �����ͺ��̽��� �⵵ �ڵ����� �ֱ� �ùٸ���
				
				if (nums[0] == 1 || nums[0] == 3 || nums[0] == 5 || nums[0] == 7
					|| nums[0] == 8 || nums[0] == 10 || nums[0] == 12) {
					if (z3 == 2) {
						printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
						return 1;
					}
					if (nums[1] >= 1 && nums[1] <= 31) {
						if (z4 == 2) {
							printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
							return 1;
						}
						// printf("���߿� �����ͺ��̽��� ��¥ ������ �ֱ�6\n"); // t�� ���̶� ��
						string y = to_string(t);
						string m = to_string(nums[0]);
						string d = to_string(nums[1]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
						return 1;
					}
				}
				else if (nums[0] == 4 || nums[0] == 6 || nums[0] == 9 || nums[0] == 11) {
					if (z3 == 2) {
						printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
						return 1;
					}
					if (nums[1] >= 1 && nums[1] <= 31) {

						//printf("���߿� �����ͺ��̽� 7\n");
						if (z4 == 2) {
							printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
							return 1;
						}
						string y = to_string(t);
						string m = to_string(nums[0]);
						string d = to_string(nums[1]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
						return 1;
					}
				}
				else if (nums[0] == 2) {
					if (z3 == 2) {
						printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
						return 1;
					}
					int k;
					k = t % 4;
					if (t % 4 == 0 && t % 100 != 0 || t % 400 == 0) {
						if (nums[1] >= 1 && nums[1] <= 29) {
							// printf("���� ��¥�ֱ�2 \n");
							if (z4 == 2) {
								printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
								return 1;
							}
							string y = to_string(t);
							string m = to_string(nums[0]);
							string d = to_string(nums[1]);
							*search_date = y + '-' + m + '-' + d;
							return 0;
						}
						else {
							printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
							return 1;
						}
					}
					else {
						if (nums[1] >= 1 && nums[1] <= 28) {
							//printf("��� ��¥ �ֱ�2\n");
							if (z4 == 2) {
								printf("���๮�� '0'�� �ʹ� �����ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
								return 1;
							}
							string y = to_string(t);
							string m = to_string(nums[0]);
							string d = to_string(nums[1]);
							*search_date = y + '-' + m + '-' + d;
							return 0;
						}
						else {
							printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
							return 1;
						}
					}
				}
				else {
					printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
					return 1;
				}
			}
			else {
				printf("���Ŀ� ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
				return 1;
			}//���̳� �����Է��Ѱ� , ���й��� �������
		}// printf("���й�������");
	}// ���й����ִ°� ����
	else {// printf("���й��ھ���");
	   //���� �⵵�Է�, �⵵ ���Է����� ���� 4�ڸ� 2�ڸ� 2�ڸ�

		int i = 0;
		int num = strlen(search_date->c_str());
		if (num == 8) {
			//year1,month1,date1
			nums[0] = year1((char*)search_date->c_str()); nums[1] = month1((char*)search_date->c_str());
			nums[2] = date1((char*)search_date->c_str());
			if ((nums[0] >= 2000 && nums[0] <= 2999)) {
				if (nums[1] == 1 || nums[1] == 3 || nums[1] == 5 || nums[1] == 7
					|| nums[1] == 8 || nums[1] == 10 || nums[1] == 12) {
					if (nums[2] >= 1 && nums[2] <= 31) {
						//printf("������ �ֱ�\n");
						string y = to_string(nums[0]);
						string m = to_string(nums[1]);
						string d = to_string(nums[2]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
						return 1;
					}

				}
				else if (nums[1] == 4 || nums[1] == 6 || nums[1] == 9 || nums[1] == 11) {
					if (nums[2] >= 1 && nums[2] <= 30) {
						//printf("������ �ֱ�\n");
						string y = to_string(nums[0]);
						string m = to_string(nums[1]);
						string d = to_string(nums[2]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
						return 1;
					}
				}
				else if (nums[1] == 2) {

					if (nums[0] % 4 == 0 && nums[0] % 100 != 0 || nums[0] % 400 == 0) {
						if (nums[2] >= 1 && nums[2] <= 29) {
							//printf("���߿� �����ͺ��̽��� ��¥�ֱ�3\n");
							string y = to_string(nums[0]);
							string m = to_string(nums[1]);
							string d = to_string(nums[2]);
							*search_date = y + '-' + m + '-' + d;
							return 0;
						}
						else {
							printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
							return 1;
						}
					}
					else {
						if (nums[2] >= 1 && nums[2] <= 28) {
							//printf("���߿� �����ͺ��̽��� ��¥�ֱ�4\n");
							string y = to_string(nums[0]);
							string m = to_string(nums[1]);
							string d = to_string(nums[2]);
							*search_date = y + '-' + m + '-' + d;
							return 0;
						}
						else {
							printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
							return 1;
						}
					}
				}
				else {
					printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
					return 1;
				}
			}
			else {
				printf("�⵵ ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
				return 1;
			}
		}
		else if (num == 4) { // month2 , date 2
			time_t base_time = time(NULL);
			struct tm* newtime = localtime(&base_time);
			int t = newtime->tm_year + 1900;
			nums[0] = month2((char*)search_date->c_str()); nums[1] = date2((char*)search_date->c_str());
			if (nums[0] == 1 || nums[0] == 3 || nums[0] == 5 || nums[0] == 7
				|| nums[0] == 8 || nums[0] == 10 || nums[0] == 12) {
				if (nums[1] >= 1 && nums[1] <= 31) {
					//printf("���� �ֱ�2\n");
					string y = to_string(t);
					string m = to_string(nums[0]);
					string d = to_string(nums[1]);
					*search_date = y + '-' + m + '-' + d;
					return 0;
				}
				else {
					printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
					return 1;
				}
			}
			else if (nums[0] == 4 || nums[0] == 6 || nums[0] == 9 || nums[0] == 11) {
				if (nums[1] >= 1 && nums[1] <= 30) {
					//printf("���� �ֱ�2\n");
					string y = to_string(t);
					string m = to_string(nums[0]);
					string d = to_string(nums[1]);
					*search_date = y + '-' + m + '-' + d;
					return 0;
				}
				else {
					printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
					return 1;
				}
			}
			else if (nums[0] == 2) {

				if (t % 4 == 0 && t % 100 != 0 || t % 400 == 0) {
					if (nums[1] >= 1 && nums[1] <= 29) {
						//printf("���� ��¥�ֱ�2 \n");
						string y = to_string(t);
						string m = to_string(nums[0]);
						string d = to_string(nums[1]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
						return 1;
					}
				}
				else {
					if (nums[1] >= 1 && nums[1] <= 28) {
						//printf("��� ��¥ �ֱ�2\n");
						string y = to_string(t);
						string m = to_string(nums[0]);
						string d = to_string(nums[1]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
						return 1;
					}
				}
			}
			else {
				printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
				return 1;
			}
		}
		else {
			printf("���ڿ��� �ùٸ��� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
			return 1;
		}

	}

	return 0;
}


int onjum(char a[]) { //(���Ʊ�) ���й��� �����Լ�
	int j = 0;
	while (a[j++] != NULL) {
		if (a[j] == '.') {
			while (a[++j] != NULL) {
				if (a[j] == '.') {
					return 1;
				}
			}
		}

	}
	return 2;
}
int slash(char a[]) { //(���Ʊ�) ���й��� �����Լ�
	int j = 0;
	while (a[j++] != NULL) {
		if (a[j] == '/') {
			while (a[++j] != NULL) {
				if (a[j] == '/') {
					return 1;
				}
			}
		}

	}
	return 2;

}
int space(char a[]) { //(���Ʊ�) ���й��� �����Լ�
	int j = 0;
	while (a[j++] != NULL) {
		if (a[j] == ' ') {
			while (a[++j] != NULL) {
				if (a[j] == ' ') {
					return 1;
				}
			}

		}
	}
	return 2;
}
int minus1(char a[]) { //(���Ʊ�) ���й��� �����Լ�
	int j = 0;
	while (a[j++] != NULL) {
		if (a[j] == '-') {
			while (a[++j] != NULL) {
				if (a[j] == '-') {
					return 1;
				}
			}

		}
	}
	return 2;
}

int year1(char a[]) { // �⵵������ �ٰ��� 6,7,8 (���Ʊ�) ���й��� ������ �⵵ ����
	int j; int num = 0;
	for (j = 0; j < 4; j++) {
		num = num * 10 + a[j] - '0';
	}
	return num;
}

int month1(char a[]) {//8�ڸ��� ��� (���Ʊ�) ���й��� ������ �� ����
	int j; int num = 0;
	for (j = 4; j < 6; j++) {
		num = num * 10 + a[j] - '0';
	}
	return num;
}

int date1(char a[]) {//8�ڸ��ϰ�� (���Ʊ�) ���й��� ������ �� ����
	int j; int num = 0;
	for (j = 6; j < 8; j++) {
		num = num * 10 + a[j] - '0';
	}
	return num;
}

int date2(char a[]) {//7�ڸ� , (���Ʊ�) ���й��� ������ ��  ����
	int j; int num = 0;
	for (j = 2; j < 4; j++) {
		num = num * 10 + a[j] - '0';
	}
	return num;
}
int month2(char a[]) { //(���Ʊ�) ���й��� ������ �� ����
	int j; int num = 0;
	for (j = 0; j < 2; j++) {
		num = num * 10 + a[j] - '0';
	}
	return num;
}



int sign(char a[]) { // (���Ʊ�)�߸��� ���� Ȯ��
	int j;  int num = 0;
	int len = strlen(a);
	for (j = 0; j < len; j++) {
		if (a[j] == '-' || a[j] == '.' || a[j] == ' ' || a[j] == '/' ||
			a[j] == '1' || a[j] == '2' || a[j] == '3' || a[j] == '4' ||
			a[j] == '5' || a[j] == '6' || a[j] == '7' || a[j] == '8' ||
			a[j] == '9' || a[j] == '0') {
			continue;
		}
		else {
			return 2;
		}
	}
	return 1;
}

int signmiss(char a[]) { //(���Ʊ�) ���й��� �������� �������� Ȯ��
	int j; int num = 0;
	int len = strlen(a);
	for (j = 0; j < len; j++) {
		if (a[j] == '-' || a[j] == '.' || a[j] == ' ' || a[j] == '/') {
			j = j + 1;
			if (a[j] == '-' || a[j] == '.' || a[j] == ' ' || a[j] == '/') {
				return 2;
			}
		}
	}
	return 1;
}

int zerogu4(char a[]) { //(���Ʊ�) �⵵�� ���๮�� 0
	int j; int num = 0;
	int len = strlen(a);
	if (a[0] == '0') {
		return 2;
	}

	return 1;
}

int zerogu1(char a[]) { //(���Ʊ�) ���๮�� '0'�� 2�� ���й��� o ,�⵵ o ��
	int j; int num = 0;
	int len = strlen(a);
	for (j = 5; j < 8; j++) {
		if (a[j] == '0') {
			if (a[++j] == '0') {
				return 2;
			}
		}
	}
	return 1;
}


int zerogu3(char a[]) { //(���Ʊ�) ���๮�� '0'�� 2�� ���й��� o ,�⵵ o ��
	int j; int num = 0;
	int len = strlen(a);
	for (j = 7; j < 11; j++) { //2020/01/100 2020/1/100
		if (a[j] == '0') {
			if (a[++j] == '0') {
				return 2;
			}
		}
	}
	return 1;
}

int zerogu5(char a[]) { //(���Ʊ�) ���๮�� '0'�� 2�� ���й��� o ,�⵵ x ��
	int j; int num = 0;
	int len = strlen(a);
	for (j = 0; j < 2; j++) {
		if (a[j] == '0') {
			if (a[++j] == '0') {
				return 2;
			}
		}
	}
	return 1;
}

int zerogu6(char a[]) { //(���Ʊ�) ���๮�� '0'�� 2�� ���й��� o ,�⵵ x ��
	int j; int num = 0;
	int len = strlen(a);
	for (j = 2; j < 5; j++) { //1/100 01/100
		if (a[j] == '0') {
			if (a[++j] == '0') {
				return 2;
			}
		}
	}
	return 1;
}

int last(char a[]) { //(���Ʊ�) ���๮�� '0'�� 2�� ���й��� o ,�⵵ x ��
	int j; int num = 0;
	int len = strlen(a)-1;
		if (a[len] == ' ' || a[len] == '-' || a[len] == '/' || a[len] == '.') {
				return 2;
	}
	return 1;
}


int isnum(char letter) {
	//���ڰ� �����̸� 1��ȯ �ƴϸ� 0 ��ȯ�ϴ� �Լ�

	int x = 1;
	char* stop; //���ڿ��� ������ ù ���ڰ� �ƴ� ���� ��ġ�� ������ ����
	strtol(&letter, &stop, 10);

	//���� ���ڸ� 0 ��ȯ
	if (letter == *stop)x = 0;
	return x;
}

//���������� �ð��� ���� �ǹ̿����� Ȯ�����ִ� �Լ�
int time_meaning_check(char* starting_time, int type) {
	int time;//���� ���� �ð���
	int hour = 00, minute;
	if (type == 1) {
		//1.hhmm
		if (strlen(starting_time) == 4 && isnum(starting_time[1]) && isnum(starting_time[2]) && isnum(starting_time[3])) {
			hour += atoi(starting_time) / 100;
			minute = atoi(starting_time) % 100;
			//printf("hhmm\n");
			//printf("%d�� %d��\n", hour, minute);
			time = hour * 100 + minute;
			if (hour > 24 || hour < 0 || minute < 0 || minute >= 60 || (hour == 24 && minute != 0)) {
				printf("�������� �ʴ� �ð��Դϴ�. �ٽ� �Է����ּ���.\n");
				return -1;
			}

		}
		//2.hh:mm and hh mm
		else if (strlen(starting_time) == 5 && (starting_time[2] == ':' || starting_time[2] == ' ') && isnum(starting_time[1]) && isnum(starting_time[3]) && isnum(starting_time[4])) {
			char hh[2], mm[2];

			hh[0] = starting_time[0];
			hh[1] = starting_time[1];
			mm[0] = starting_time[3];
			mm[1] = starting_time[4];

			hour += atoi(hh);
			minute = atoi(mm);
			//printf("hh:mm hh mmm\n");
			//printf("%d�� %d��\n", hour, minute);
			time = hour * 100 + minute;
			if (hour > 24 || hour < 0 || minute < 0 || minute >= 60 || (hour == 24 && minute != 0)) {
				printf("�������� �ʴ� �ð��Դϴ�. �ٽ� �Է����ּ���.\n");
				return -1;
			}

		}
		//3. ������ ������ �ƴҶ�
		else {
			printf("�ùٸ� �ð������� �ƴմϴ�. \"hh:mm\", \"hhmm\", \"hh mm\" �� �ϳ��� �������� �Է����ּ���.\n");
			return -1;
		}
	}
	else if (type == 2 || type == 3) { //type2 -> �϶� 00:00~12:00���� �Է¹���
		//1.hhmm
		if (strlen(starting_time) == 4 && isnum(starting_time[1]) && isnum(starting_time[2]) && isnum(starting_time[3])) {
			if (type == 3) hour += atoi(starting_time) / 100 + 12;	//type3�ΰ�� 12 ���ϱ�
			else hour += atoi(starting_time) / 100;
			minute = atoi(starting_time) % 100;
			//printf("hhmm\n");
			//printf("%d�� %d��\n", hour, minute);
			time = hour * 100 + minute;
			if (type == 2 && (hour > 12 || hour < 0 || minute < 0 || minute >= 60 || (hour == 12 && minute != 0))) {
				printf("�������� �ʴ� �ð��Դϴ�. �ٽ� �Է����ּ���.\n");
				return -1;
			}
			if (type == 3 && (hour > 24 || hour < 0 || minute < 0 || minute >= 60 || (hour == 24 && minute != 0))) {
				printf("�������� �ʴ� �ð��Դϴ�. �ٽ� �Է����ּ���.\n");
				return -1;
			}

		}

		//2.hh:mm and hh mm
		else if (strlen(starting_time) == 5 && (starting_time[2] == ':' || starting_time[2] == ' ') && isnum(starting_time[1]) && isnum(starting_time[3]) && isnum(starting_time[4])) {
			char hh[2], mm[2];

			hh[0] = starting_time[0];
			hh[1] = starting_time[1];
			mm[0] = starting_time[3];
			mm[1] = starting_time[4];

			if (type == 3) hour += atoi(hh) + 12;	//type3�ΰ�� 12 ���ϱ�
			else hour += atoi(hh);
			minute = atoi(mm);
			//printf("hh:mm hh mmm\n");
			//printf("%d�� %d��\n", hour, minute);
			time = hour * 100 + minute;
			if (type == 2 && (hour > 12 || hour < 0 || minute < 0 || minute >= 60 || (hour == 12 && minute != 0))) {
				printf("�������� �ʴ� �ð��Դϴ�. �ٽ� �Է����ּ���.\n");
				return -1;
			}
			if (type == 3 && (hour > 24 || hour < 0 || minute < 0 || minute >= 60 || (hour == 24 && minute != 0))) {
				printf("�������� �ʴ� �ð��Դϴ�. �ٽ� �Է����ּ���.\n");
				return -1;
			}


		}

		//3. ������ ������ �ƴҶ�
		else {
			printf("�ùٸ� �ð������� �ƴմϴ�. \"hh:mm\", \"hhmm\", \"hh mm\" �� �ϳ��� �������� �Է����ּ���.\n");
			return -1;
		}
	}

	return time;

}





void time_grammar_check(int start_or_end, string& time_in_string) {

	char time_input[50];		// �Է¹��� �ð� ���ڿ�
	char first_letter;
	int hour, minute;
	int start_time = 0;		// DB���Ŀ� �°� ������ �ð���
	int type; //1:���ڷθ� �̷���� ���ڿ�, 2:����,am,AM�� ���Ե� �ð����ڿ�, 3:����,pm,PM�� ���Ե� �ð����ڿ�
	int time = -1; //DB�ð� ���¿� �°� ������ �ð� �� �̰��� �����ؾ���

	rewind(stdin); // �׿� Ű�� �ʱ�ȭ
	while (1) {
		if (start_or_end == 0) {
			printf("���۽ð��� �Է����ּ���: ");

			//#### ���μ��� ���� ����� (����ȣ)
			try {
				if (gets_s(time_input) == 0)
					throw start_time;
			}
			catch (int err) {
				printf("��� ������ �Է����� �ʾ� ������� �ʾҽ��ϴ�.\n");
				Sleep(3000);
				exit(1);
			}
		}
		else {
			printf("�����ð��� �Է����ּ���: ");
			//#### ���μ��� ���� ����� (����ȣ)
			try {
				if (gets_s(time_input) == 0)
					throw start_time;
			}
			catch (int err) {
				printf("��� ������ �Է����� �ʾ� ������� �ʾҽ��ϴ�.\n");
				Sleep(3000);
				exit(1);
			}
		}


		//���ڷθ� �̷���� �ð� ǥ��: ù���ڰ� �������� �������� ���� �Ǵ�
		if (isnum(time_input[0])) {
			//���� �϶�
			//printf("���ڷθ� �̷���� �ð� ǥ��\n");
			type = 1; //���ڿ� type ����
			time = time_meaning_check(time_input, type);


		}

		else {//���ڰ� ���Ե� �ð� ǥ��:
			//printf("���� ���Ե� �ð�ǥ��");
			if (strstr(time_input, "���� ") == time_input || strstr(time_input, "���� ") == time_input) {
				//���ڿ� type ����
				if (strstr(time_input, "���� ") == time_input) type = 3;
				else type = 2;

				//���ڸ� ������ �ð� �κ� ����
				for (int i = 5; i < 50; i++) {
					time_input[i - 5] = time_input[i];
				}
				//�ǹ̿��� Ȯ��
				if (isnum(time_input[0])) {
					time = time_meaning_check(time_input, type);

				}
				//��������: ���ڰ� �ƴ� ���ڿ��� ���
				else {
					printf("���� ���·� �ð��� �Է����ּ���! \"hh:mm\", \"hhmm\", \"hh mm\" �� �ϳ��� ���ĸ� �����մϴ�.\n");
				}
			}
			else if (strstr(time_input, "am ") == time_input || strstr(time_input, "AM ") || strstr(time_input, "pm ") == time_input || strstr(time_input, "PM ") == time_input) {
				//���ڿ� type ���� 
				if (strstr(time_input, "pm ") == time_input || strstr(time_input, "PM ") == time_input) type = 3;
				else type = 2;

				//���ڸ� ������ �ð� �κ� ����
				for (int i = 3; i < 50; i++) {
					time_input[i - 3] = time_input[i];
				}
				//�ǹ̿��� Ȯ��
				if (isnum(time_input[0])) {
					time = time_meaning_check(time_input, type);

				}
				//��������: ���ڰ� �ƴ� ���ڿ��� ���
				else {
					printf("���� ���·� �ð��� �Է����ּ���! \"hh:mm\", \"hhmm\", \"hh mm\" �� �ϳ��� ���ĸ� �����մϴ�.\n");
				}
			}
			else {
				printf("��� �Ұ����� ���ڿ��Դϴ�! ���ڴ� \"����\", \"am\",\"AM\",\"����\",\"pm\",\"PM\" �� �ϳ��� ��밡�� �մϴ�. �ش� ���ڿ� �Է����� �� ĭ�� ��� �ð��� �Է����ּ���.\n");
			}
		}
		if (time != -1) break;	//time���� ���� ���ϵ� ��� �ݺ��� Ż��
	}
	//printf("%d", time);


	if (time == 0) 	time_in_string = "0000";
	else if (time < 100) time_in_string = "00" + to_string(time);
	else if (time < 1000)		time_in_string = "0" + to_string(time);
	else time_in_string = to_string(time);

}

int time_interval_check1(vector<string>* db, string time_in_string, int start_or_end) {//�Էµ� �ð��� ����� �ð����� �ȿ� ���ԵǴ��� ���� Ȯ��

	int time = stoi(time_in_string);//�ð� ���ڿ��� int������ �ٲپ��� ��, ���� ������ �ʰ� �ϳ��� ������ ũ��� ����
	stringstream stream_test;

	int num, year, month, day, start_time, end_time;

	int length = (*db).size();
	for (int i = 0; i < length; i++)
	{
		stream_test.clear(); // �ٽ� �ʱ�ȭ
		stream_test.str((*db).at(i)); // �̹��� �ش� ������ DB�� ������.

		stream_test >> num >> year >> month >> day >> start_time >> end_time;
		if (start_or_end == 1) {	//�����ð��ΰ��
			if (time > start_time && time <= end_time) {// ����� �ð������ȿ� �����ϴ� �ð��Է�
				printf("�ش� �ð��� �̹� �Էµ� Ȱ���� �ֽ��ϴ�. \n");
				return 1;
			}
		}

		if (start_or_end == 0) {	//���۽ð��� ���
			if (time >= start_time && time < end_time) {// ����� �ð������ȿ� �����ϴ� �ð��Է�
				printf("�ش� �ð��� �̹� �Էµ� Ȱ���� �ֽ��ϴ�. \n");
				return 1;
			}
		}
	}
	return 0;

}


int time_interval_check2(vector<string>* db, string start_time_in_string, string end_time_in_string) { //�Էµ� �ð������ȿ� �ռ� ����Ǿ� �ִ� Ȱ���� ���ԵǴ��� ���� Ȯ��
	int initial_time = stoi(start_time_in_string);//�Էµ� ���۽ð��ð� ���ڿ��� int������ �ٲپ��� ��, ���� ������ �ʰ� �ϳ��� ������ ũ��� ����
	int final_time = stoi(end_time_in_string);	//�Էµ� �����ð�

	stringstream stream_test;

	int num, year, month, day, start_time, end_time;

	int length = (*db).size();

	for (int i = 0; i < length; i++) {
		stream_test.clear(); // �ٽ� �ʱ�ȭ
		stream_test.str((*db).at(i)); // �̹��� �ش� ������ DB�� ������.

		stream_test >> num >> year >> month >> day >> start_time >> end_time;
		if ((initial_time <= start_time && final_time > start_time) || (initial_time < end_time && final_time >= end_time)) {// ����� �ð������ȿ� �����ϴ� �ð��Է�
			printf("�Էµ� �ð����� ���̿� �̹� �Էµ� Ȱ���� �ֽ��ϴ�. \n");
			return 1;
		}
	}
	return 0;
}


void DailyChart::Schedule_make(vector<string>* db, string date_data) {

	/* ���� ����

	vector<string> db : �ش� ������ ���� ������ ����� DB ������ ����.

	string date_data : ������ �ϰ� ��¥ ����. ���� �ޱ�� string ���·� �޾���.

	*/
	rewind(stdin); // �׿� Ű�� �ʱ�ȭ

	vector<string> DB_test;

	All_Database.clear();
	Daily_DB_Read(&All_Database); // ���� ��ü DB ����

	int D_year = 0, D_month = 0, D_day = 0;
	int last_num; // db�� �ִ� ������ ������ Id_num(������ȣ)�� ���� ����. last_num + 1 �� ������ȣ�� �����ϱ� ���ؼ���.
	int i = 0;
	//int s = 0;//ī�װ� ���� �˻� ����
	char a = 'a';
	char test[10000];
	int test_option= 0;

	stringstream stream_test;
	stream_test.str(date_data);


	stream_test >> D_year >> D_month >> D_day; // ����� ����
	stream_test.clear();

	/*stringstream stream_test2;*/
	if (All_Database.back().empty() == 0) // ���������� ���� �ȵǾ�������
	{
		test_option = 1;
	}
	else // ���͵Ǿ������� ���͵� �󰪸� ��.
	{
		All_Database.pop_back();
	}
	
	stream_test.str((All_Database).back()); // ��ü DB�� (������-1)�� ����. => ������ P_ID�� �����ϱ� ����. ���������� ��ĭ��.

	stream_test >> num; // ���� P_ID ����


	last_num = num;

	D_year = abs(D_year);
	D_month = abs(D_month);
	D_day = abs(D_day);


	num = last_num + 1; // ���� �Է��� ������ ������ P_ID�� +1�� ����


	//#### ���⼭ ��ġ�� �ð��밡 �ִ��� üũ�ؾ���.

	//�ð� �Է�(������)
	while (1) {
		while (1) {

			time_grammar_check(0, start_time);	// ��������, �ǹ̿��� �˻� �� �ð� ǥ�� ǥ��ȭ��Ŵ
			if (time_interval_check1(db, start_time, 0) == 0) break;	//����� �ð����� �ȿ� ���ԵǴ��� ���� Ȯ��
		}
		while (1) {
			time_grammar_check(1, end_time);	// ��������, �ǹ̿��� �˻� �� �ð� ǥ�� ǥ��ȭ��Ŵ
			if (stoi(end_time) <= stoi(start_time)) {//�����ð��� ���۽ð� �ڿ� ������ Ȯ��
				printf("�����ð��� ���۽ð����� �ڿ����� �ð��̾���մϴ�.\n");
				continue;
			}
			if (time_interval_check1(db, end_time, 1) == 0) break;	//����� �ð����� �ȿ� ���ԵǴ��� ���� Ȯ��
		}
		if (time_interval_check2(db, start_time, end_time) == 0) break;
	}


	// �ð� ���� ���

	/*
	�� �Լ����� �ð��� '0600', "06:00"(6�� ����) ���� ���·� �޾Ƶ鿴�ٴ� ���� ������ �Ѵ�. �� �ܴ̿� �������� �ʴ´�.
	�׷��� ����ó���� ���� ���·� ǥ��ȭ�� ��Ŵ.
	*/
	stream_test.clear();


	int start_minute, end_minute, start_hour, end_hour;
	start_hour = atoi(start_time.substr(0, 2).c_str()); // �� �ÿ� �����ߴ����� string���� int�� ��ȯ
	end_hour = atoi(end_time.substr(0, 2).c_str()); // ���� �ø� string���� int�� ��ȯ
	start_minute = atoi(start_time.substr(start_time.length() - 2, 2).c_str()); // ���� �� string���� int�� ��ȯ
	end_minute = atoi(end_time.substr(start_time.length() - 2, 2).c_str()); // ���� �� string���� int�� ��ȯ


	/*
	�Լ� ����

	atoi(char *) : char *�� �迭�� int�� ��ȯ����

	c_str() : C++�� string��  C����� char* �� �迭�� �ٲ���

	*/


	btw_time = ReturnBtwTime(start_minute, end_minute, start_hour, end_hour);



	//ī�װ�(������ & ����ȣ)


	string t;
	stringstream s1;
	int p;

	while (1) {
		rewind(stdin); // �׿� Ű�� �ʱ�ȭ
		test[0] = '\0'; // �����ʱ�ȭ

		printf("Ȱ���� ������ �ּ���(1.����, 2.�Ļ�, 3.����, 4.����, 5.�, 6.���, 7.�Ƹ�����Ʈ, 8.��Ÿ): ");

		//#### ���� ���� ����ó��(����ȣ)
		try {
			if (gets_s(test) == 0)
			{
				throw end_time;
			}
		}
		catch (string err2) {
			printf("��� ������ �Է����� �ʾ� ������� �ʾҽ��ϴ�.\n");
			Sleep(3000);
			exit(1);
		}

		cartegory = test;


		if (cartegory == "1") {
			cartegory = "����";
			break;
		}
		else if (cartegory == "2") {
			cartegory = "�Ļ�";
			break;
		}
		else if (cartegory == "3") {
			cartegory = "����";
			break;
		}
		else if (cartegory == "4") {
			cartegory = "����";
			break;
		}
		else if (cartegory == "5") {
			cartegory = "�";
			break;
		}
		else if (cartegory == "6") {
			cartegory = "���";
			break;
		}
		else if (cartegory == "7") {
			cartegory = "�Ƹ�����Ʈ";
			break;
		}
		else if (cartegory == "8") {
			cartegory = "��Ÿ";
			break;
		}
		else if (atoi(cartegory.c_str()) == 0 && cartegory != "0") { // atoi�� �ؼ��Ǵ� int���� �����ϰ�, �ؼ��� ������ �� 0�� �����Ѵ�.
			// �� 0���� �����ߴٴ� ���� ���ڿ��� �Է� �޾Ҵٴ� �Ҹ���.
			printf("'%s'�� ���ڿ� �Դϴ�! 1-8������ ���ڸ� �Է����ּ���.:\n", cartegory.c_str());

		}
		else { // �������� ���� ���ڸ� ���� ���ڿ��̰ų�(ex. 1������), ������ ���� �ʴ� ���ڵ��̴�.
			

			s1.clear();
			t.clear();

			s1.str(cartegory);

			if (detail_chart.size() > 1) // 1���� �̻��̸� -> ���ڿ��� Ȯ�� ����
			{

				s1 >> p >> t;

				if (t.empty()) // string�� ��������� ī�װ��� ���ڿ�
				{
					printf("�߸��� �Է��Դϴ�. 1���� 8������ ���ڸ� �Է����ּ���.\n");
				}
				else // �ƴϸ� ���ڿ�
				{
					printf("'%s'�� ���ڿ� �Դϴ�! 1-8������ ���ڸ� �Է����ּ���.:\n", cartegory.c_str());
				}
			}
			else
			{
				printf("�߸��� �Է��Դϴ�. 1���� 8������ ���ڸ� �Է����ּ���.\n");
			}
		
			/*
			for (i = 0; i < cartegory.size(); ++i) {

				if (!isdigit(cartegory[i])) {//isdigit()�Ű������� ���ڸ� 0�� �ƴ� �� �Ű������� ���ڰ� �ƴϸ� 0�� ��ȯ

					++s;

				}

			}
			if (s > 0) {
				printf("'%s'�� ���ڿ� �Դϴ�! 1-8������ ���ڸ� �Է����ּ���.:\n", cartegory.c_str());
				s = 0;
			}
			else {

				printf("�߸��� �Է��Դϴ�. 1���� 8������ ���ڸ� �Է����ּ���.\n");
			}*/

		}

	}

	rewind(stdin); // �׿� Ű�� �ʱ�ȭ 


	//���γ���

	while (1) {
		cout << "���γ����� ������ : ";
		test[0] = '\0'; // ���� �ʱ�ȭ

		//#### ���μ��� ���� ����� (����ȣ)
		try {
			//std::getline(cin, detail_chart);
			if (gets_s(test) == 0) {
				throw a;
			}

			cout << "" << endl;
		}
		catch (char err) {
			printf("��� ������ �Է����� �ʾ� ������� �ʾҽ��ϴ�.\n");
			Sleep(3000);
			exit(1);
		}

		detail_chart = test;

		//���γ��� �����˻�

		if (CheckDetail(&detail_chart)) { // �����˻� ����ϸ� 1�� ���ϵǼ� break��.

			break;
		}
	}


	string str;

	// ���� ������ �ϳ��� string �����

	if (test_option) // �������࿡ ���Ͱ� ������ �־�����
	{
		str = '\n' + to_string(num) + '\t' + to_string(D_year) + '\t' + to_string(D_month) + '\t' + to_string(D_day) + '\t' +
			start_time + '\t' + end_time + '\t' + to_string(btw_time) + '\t' + cartegory + '\t' + detail_chart + '\n';
	}
	else //���� ������
	{
		str = to_string(num) + '\t' + to_string(D_year) + '\t' + to_string(D_month) + '\t' + to_string(D_day) + '\t' +
			start_time + '\t' + end_time + '\t' + to_string(btw_time) + '\t' + cartegory + '\t' + detail_chart + '\n';
	}

	
	// ���� �̾� ����

	ofstream WriteFile("DB.txt", ios::app);

	WriteFile << str;

	printf("����Ǿ����ϴ�!\n");

	WriteFile.close();

	rewind(stdin); // �׿� Ű�� �ʱ�ȭ 
	while (!kbhit()) // �ƹ�Ű �Է� ������ ����Ǵ� ��
	{

	}

}


int DailyChart::CheckP_ID(vector<string>* db, string id) {// P_ID ���� üũ �Լ� ####
	/*������ ���� ���ϰ��� �޸� �� ��*/

	stringstream stream_test;

	int ex_check = 0; // �ش� ���ڿ� �ش� P_ID�� �����ϴ� �� ���� üũ ����
	int i, s = 0;// ������ȣ ���� �˻� ����


	/*####�켱 P_ID�� ������ ������ Ȯ��*/

	if (atoi(id.c_str()) == 0 && id != "0") {// ������ ������ (���ڿ��� �Է� �޾��� ��)
		printf("�������� �ʴ� ������ȣ �Դϴ�.\n");
		return 0;
	}
	else {
		for (i = 0; i < id.size(); ++i) {

			if (!isdigit(id[i])) {//isdigit()�Ű������� ���ڸ� 0�� �ƴ� �� �Ű������� ���ڰ� �ƴϸ� 0�� ��ȯ

				++s; // ���ڰ� ������ ī��Ʈ�� �ö�

			}

		}

		if (s > 0) { // �̰� '1������' ���� ���ο� ���ڰ� ���Ե� ���ڿ�
			printf("�������� �ʴ� ������ȣ �Դϴ�.\n");
			s = 0;
			return 0;
		}
	}

	//�̰ɷ� ���ڸ� ������ �ɷ���


	// ���� P_ID�� �ش� ���ڿ��� �����ϴ� ��ȣ���� Ȯ��.

	for (unsigned int i = 0; i < (*db).size(); i++) {// db ũ�� ��ŭ ��� ���� ������.
		/*
		���� DB�� db[0] = "1\t2020\t03\t20\t0600\t0900\t180\t�Ļ�\t��ħ�Ļ�"
		�̷� ������ ������ �Ǿ��ִ�.

		���̺� ������ ������� int, int, int, int, string, string, int, string, string �̹Ƿ�

		�� ������� sstream�� �̿��� ���ڸ� �޴´�.

		���⼭ vector�� string ���̱⿡ db[i]�� ��� ������ �� string ���� ���� �������.
		*/


		stream_test.clear(); // sstream�� �ѹ� ���� ���� �ٽ� �ʱ�ȭ ���Ѿ� �� �� �ִ�.

		stream_test.str((*db)[i]);

		stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
			cartegory >> detail_chart; // Ŭ���� ���� ����


		if (atoi(id.c_str()) == num) {// ������ȣ�� ��ġ�ϸ�
			ex_check = 1;
			// �ش� ��ȣ�� �ϳ��� �����ϹǷ� for���� ����.
			break; // ������ȣ�� �ϳ� �ۿ� ���� ������ break�ؾ� �޸� �Ƴ�.

		}
		else { // ��ġ ���ϴ� ������
			//�ϴ� �Ѿ.

		}


	}

	if (ex_check) {// �����ϴ� ��ȣ��
		return 1;
	}
	else {// �������� �ʴ� ��ȣ��
		//�������� �ʴ´ٰ� ���
		printf("�������� �ʴ� ������ȣ�Դϴ�.\n");
		return 0;
	}

}

void DailyChart::Schedule_delete(vector<string>* db, string date_data)// ���� ���� �Լ�
{
	/* ���� ���� ����

	���ϴ� P_ID�� �Է¹ް� ��ü DB���� �ش� ���� ã�� ������ ��,
	���� DB�� ���׸� ����� ����̴�.

	*/

	rewind(stdin); // �׿� Ű�� �ʱ�ȭ

	vector<string> DB_test; // ���⿡ ������ ���� ���� �ٽ� �� ������ ����.

	All_Database.clear();

	Daily_DB_Read(&All_Database); // ���� ��ü DB ����

	int D_year = 0, D_month = 0, D_day = 0;
	string p_id;
	int index_count = 0;

	stringstream stream_test;
	stream_test.str(date_data);

	char test[10000];

	stream_test >> D_year >> D_month >> D_day; // ����� ����
	stream_test.clear();

	D_year = abs(D_year);
	D_month = abs(D_month);
	D_day = abs(D_day);

	if ((*db).size() != 0) {
		while (1) {
			printf("�����Ͻ� Ȱ���� ������ȣ�� �������ּ���: ");

			//#### ���μ��� ���� ����� (����ȣ)
			try {
				//std::getline(cin, p_id);

				if (gets_s(test) == 0) {
					throw test;
				}

				cout << "" << endl;
			}
			catch (char *err) {
				printf("��� ������ �Է����� �ʾ� ������� �ʾҽ��ϴ�.\n");
				Sleep(3000);
				exit(1);
			}

			//p_id ���� �˻�

			p_id = test;

			if (CheckP_ID(db, p_id)) { // ������ ������
				break;
			}
			else {// ������ ������
				//���� ���
			}
		}

		(*db).clear();

		//p_id�� �´� �� ��ȣ�� ã�� ���� ��� DB�� ������.

		for (unsigned int i = 0; i < (All_Database).size() - 1; i++) {
			/*
			���� DB�� db[0] = "1\t2020\t03\t20\t0600\t0900\t180\t�Ļ�\t��ħ�Ļ�"
			�̷� ������ ������ �Ǿ��ִ�.

			���̺� ������ ������� int, int, int, int, string, string, int, string, string �̹Ƿ�

			�� ������� sstream�� �̿��� ���ڸ� �޴´�.

			���⼭ vector�� string ���̱⿡ db[i]�� ��� ������ �� string ���� ���� �������.
			*/


			stream_test.clear(); // sstream�� �ѹ� ���� ���� �ٽ� �ʱ�ȭ ���Ѿ� �� �� �ִ�.

			stream_test.str((All_Database)[i]);

			stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
				cartegory >> detail_chart; // Ŭ���� ���� ����


			if (atoi(p_id.c_str()) == num) {// ������ȣ�� ��ġ�ϸ�
				// �̰͸� �������� ����.

			}
			else { // ��ġ ���ϴ� ������
				if (D_year == year && D_month == month && D_day == day)
					(*db).push_back(All_Database[i]);

				(DB_test).push_back(All_Database[i]); // �о�� ������ DB_test�� �������� ����
			}


		}



		// ������ ��ģ DB�� ��������� ���� �̸� ����� �ȴ�.

		ofstream WriteFile_1("DB.txt", ios::out); // out�� ���� ������ ���� �����ϰ� ó������ ���ڴ� �ǹ�
		string str = DB_test[0] + '\n';

		WriteFile_1 << str;
		WriteFile_1.close();


		for (unsigned int i = 1; i < (DB_test).size(); i++) {
			ofstream WriteFile_2("DB.txt", ios::app); //app�� ���� ���뿡 �̾�ڴ� �ǹ�.
			str = DB_test[i] + '\n';
			WriteFile_2 << str;
			WriteFile_2.close();
		}


		printf("����Ǿ����ϴ�!\n");

		rewind(stdin); // �׿� Ű�� �ʱ�ȭ 
		while (!kbhit()) // �ƹ�Ű �Է� ������ ����Ǵ� ��
		{

		}
	}
}


void DailyChart::Schedule_modify(vector<string>* db, string date_data) {// ���� ���� �Լ�
	/* ���� ���� ����

	��ġ�ϴ� ��¥�� ã�� �ش� DB���ڸ� �ٲٰ�
	�̸� �ٽ� ����� ����̴�.
	*/

	/* ���� ����

	vector<string> db : �ش� ������ ���� ������ ����� DB ������ ����.

	string date_data : ������ �ϰ� ��¥ ����. ���� �ޱ�� string ���·� �޾���.

	*/


	rewind(stdin); // �׿� Ű�� �ʱ�ȭ

	All_Database.clear();
	Daily_DB_Read(&All_Database); // ���� ��ü DB ����

	vector<string> DB_test; // �ش� ������ DB���� ������ �� �� ������ DB

	int D_year = 0, D_month = 0, D_day = 0;
	string p_id; // ������ȣ �Է� ����
	int id_num; // ������ȣ�� �´� ������ ������� DB �� ��ȣ ����
	int i = 0;
	// int s = 0;//ī�װ� ���� �˻� ����

	char test[10000];
	char a = 'a';
	float b = 0.0;

	stringstream stream_test;
	stream_test.str(date_data);

	stream_test >> D_year >> D_month >> D_day; // ����� ����
	stream_test.clear();

	D_year = abs(D_year);
	D_month = abs(D_month);
	D_day = abs(D_day);

	if ((*db).size() != 0) {
		while (1) {

			printf("�����Ͻ� Ȱ���� ������ȣ�� �������ּ���: ");

			//#### ���μ��� ���� ����� (����ȣ)
			try {
				//std::getline(cin, p_id);

				if (gets_s(test) == 0) {
					throw test;
				}

				cout << "" << endl;
			}
			catch (char *err) {
				printf("��� ������ �Է����� �ʾ� ������� �ʾҽ��ϴ�.\n");
				Sleep(3000);
				exit(1);
			}

			p_id = test;

			//#### p_id ���� �˻�


			if (CheckP_ID(db, p_id)) {// ������ ������
				break;
			}

		}

		//p_id�� �´� �� ��ȣ�� ã�� ���� ��� DB�� ������.

		for (unsigned int i = 0; i < (All_Database).size() - 1; i++) {
			/*
			���� DB�� db[0] = "1\t2020\t03\t20\t0600\t0900\t180\t�Ļ�\t��ħ�Ļ�"
			�̷� ������ ������ �Ǿ��ִ�.

			���̺� ������ ������� int, int, int, int, string, string, int, string, string �̹Ƿ�

			�� ������� sstream�� �̿��� ���ڸ� �޴´�.

			���⼭ vector�� string ���̱⿡ db[i]�� ��� ������ �� string ���� ���� �������.
			*/


			stream_test.clear(); // sstream�� �ѹ� ���� ���� �ٽ� �ʱ�ȭ ���Ѿ� �� �� �ִ�.

			stream_test.str((All_Database)[i]);

			stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
				cartegory >> detail_chart; // Ŭ���� ���� ����

			if (D_day == day && D_month == month && D_year == year) {
				if (atoi(p_id.c_str()) == num) {// ������ȣ�� ��ġ�ϸ�
					//DB�� �ش� ���ȣ�� �����ص�.
					id_num = i;

				}
				else {
					DB_test.push_back(All_Database[i]); // ������ ���� ���� DB ����
				}
			}

		}




		/* ���Ĵ� Scheduel_make �Լ��� �����Ͽ� �ۼ�.*/


		//�ð� �Է�(������)
		while (1) {

			while (1) {

				time_grammar_check(0, start_time);	// ��������, �ǹ̿��� �˻� �� �ð� ǥ�� ǥ��ȭ��Ŵ
				if (time_interval_check1(&DB_test, start_time, 0) == 0) break;	//����� �ð����� �ȿ� ���ԵǴ��� ���� Ȯ��
			}
			while (1) {
				time_grammar_check(1, end_time);	// ��������, �ǹ̿��� �˻� �� �ð� ǥ�� ǥ��ȭ��Ŵ
				if (stoi(end_time) <= stoi(start_time)) {//�����ð��� ���۽ð� �ڿ� ������ Ȯ��
					printf("�����ð��� ���۽ð����� �ڿ� ���� �ð��̾���մϴ�.\n");
					continue;
				}
				if (time_interval_check1(&DB_test, end_time, 1) == 0) break;	//����� �ð����� �ȿ� ���ԵǴ��� ���� Ȯ��
			}
			if (time_interval_check2(&DB_test, start_time, end_time) == 0) break;
		}



		stream_test.clear();


		int start_minute, end_minute, start_hour, end_hour;
		start_hour = atoi(start_time.substr(0, 2).c_str()); // �� �ÿ� �����ߴ����� string���� int�� ��ȯ
		end_hour = atoi(end_time.substr(0, 2).c_str()); // ���� �ø� string���� int�� ��ȯ
		start_minute = atoi(start_time.substr(start_time.length() - 2, 2).c_str()); // ���� �� string���� int�� ��ȯ
		end_minute = atoi(end_time.substr(start_time.length() - 2, 2).c_str()); // ���� �� string���� int�� ��ȯ


		/*
		�Լ� ����

		atoi(char *) : char *�� �迭�� int�� ��ȯ����

		c_str() : C++�� string��  C����� char* �� �迭�� �ٲ���

		*/

		btw_time = ReturnBtwTime(start_minute, end_minute, start_hour, end_hour);




		//ī�װ�(������ & ����ȣ)

		string t;
		stringstream s1;
		int p;


		while (1) {
			rewind(stdin); // �׿� Ű�� �ʱ�ȭ
			test[0] = '\0'; // ���� �ʱ�ȭ
			cout << "Ȱ���� ������ �ּ���(1.����, 2.�Ļ�, 3.����, 4.����, 5.�, 6.���, 7.�Ƹ�����Ʈ, 8.��Ÿ): ";

			//#### ���� ���� ����ó��
			try {
				//std::getline(cin, cartegory);
				if (gets_s(test) == 0) {
					throw start_time;
				}
			}
			catch (string err) {
				printf("��� ������ �Է����� �ʾ� ������� �ʾҽ��ϴ�.\n");
				Sleep(3000);
				exit(1);
			}

			cartegory = test;

			if (cartegory == "1") {
				cartegory = "����";
				break;
			}
			else if (cartegory == "2") {
				cartegory = "�Ļ�";
				break;
			}
			else if (cartegory == "3") {
				cartegory = "����";
				break;
			}
			else if (cartegory == "4") {
				cartegory = "����";
				break;
			}
			else if (cartegory == "5") {
				cartegory = "�";
				break;
			}
			else if (cartegory == "6") {
				cartegory = "���";
				break;
			}
			else if (cartegory == "7") {
				cartegory = "�Ƹ�����Ʈ";
				break;
			}
			else if (cartegory == "8") {
				cartegory = "��Ÿ";
				break;
			}
			else if (atoi(cartegory.c_str()) == 0 && cartegory != "0") {// atoi�� �ؼ��Ǵ� int���� �����ϰ�, �ؼ��� ������ �� 0�� �����Ѵ�.  �� 0���� �����ߴٴ� ���� ���ڿ��� �Է� �޾Ҵٴ� �Ҹ���.
				printf("'%s'�� ���ڿ� �Դϴ�! 1-8������ ���ڸ� �Է����ּ���.:\n", cartegory.c_str());

			}
			else {// �������� ���� ���ڸ� ���� ���ڿ��̰ų�(ex. 1������), ������ ���� �ʴ� ���ڵ��̴�.
				
				s1.clear();
				t.clear();

				s1.str(cartegory);

				if (detail_chart.size() > 1) // 1���� �̻��̸� -> ���ڿ��� Ȯ�� ����
				{

					s1 >> p >> t;

					if (t.empty()) // string�� ��������� ī�װ��� ���ڿ�
					{
						printf("�߸��� �Է��Դϴ�. 1���� 8������ ���ڸ� �Է����ּ���.\n");
					}
					else // �ƴϸ� ���ڿ�
					{
						printf("'%s'�� ���ڿ� �Դϴ�! 1-8������ ���ڸ� �Է����ּ���.:\n", cartegory.c_str());
					}
				}
				else
				{
					printf("�߸��� �Է��Դϴ�. 1���� 8������ ���ڸ� �Է����ּ���.\n");
				}
				/*
				for (i = 0; i < cartegory.size(); ++i) {

					if (!isdigit(cartegory[i])) {//isdigit()�Ű������� ���ڸ� 0�� �ƴ� �� �Ű������� ���ڰ� �ƴϸ� 0�� ��ȯ

						++s;

					}

				}
				if (s > 0) {
					printf("'%s'�� ���ڿ� �Դϴ�! 1-8������ ���ڸ� �Է����ּ���.:\n", cartegory.c_str());
					s = 0;
				}
				else {

					printf("�߸��� �Է��Դϴ�. 1���� 8������ ���ڸ� �Է����ּ���.\n");
				}*/

			}
		}





		//���γ���

		while (1) {
			rewind(stdin); // �׿� Ű�� �ʱ�ȭ 
			test[0] = '\0';
			printf("���γ����� �Է����ּ��� : ");
			//cin >> detail_chart; 
			//cout << "" << endl;

			//#### ���μ��� ���� ����� (����ȣ) 
			try {

				if (gets_s(test) == 0) {
					throw a;
				}

				cout << "" << endl;
			}
			catch (char err) {
				printf("��� ������ �Է����� �ʾ� ������� �ʾҽ��ϴ�.\n");
				Sleep(3000);
				exit(1);
			}

			detail_chart = test;

			//#### ���γ��� �����˻��ؾ���

			if (CheckDetail(&detail_chart)) { // �����˻� ����ϸ� 1�� ���ϵǼ� break��.
				break;
			}

		}



		// ���� ������ ���� DB�� �����ϱ�.

		All_Database[id_num] = p_id + '\t' + to_string(year) + '\t' + to_string(month) + '\t' + to_string(day) + '\t' +
			start_time + '\t' + end_time + '\t' + to_string(btw_time) + '\t' + cartegory + '\t' + detail_chart;


		// ������ ��ģ DB�� ��������� ���� �̸� ����� �ȴ�.

		ofstream WriteFile_1("DB.txt", ios::out); // out�� ���� ������ ���� �����ϰ� ó������ ���ڴ� �ǹ�
		string str = All_Database[0] + '\n';

		WriteFile_1 << str;
		WriteFile_1.close();


		for (unsigned int i = 1; i < (All_Database).size() - 1; i++) {
			ofstream WriteFile_2("DB.txt", ios::app); //app�� ���� ���뿡 �̾�ڴ� �ǹ�.
			str = All_Database[i] + '\n';
			WriteFile_2 << str;
			WriteFile_2.close();
		}


		printf("����Ǿ����ϴ�!\n");



		rewind(stdin); // �׿� Ű�� �ʱ�ȭ 
		while (!kbhit()) // �ƹ�Ű �Է� ������ ����Ǵ� ��
		{

		}
	}
}







void menu_1() {
	rewind(stdin); // �׿� Ű�� �ʱ�ȭ
	system("cls");
	printf("*�Ϸ� �ϰ� �Է� �� ����*\n\n");

	int wrong_1 = 0; //���� Ȯ�� ����
	string write_date; // �Է��ҳ�¥
	string write_dateo;
	DailyChart D_1; // Ŭ���� ����

	string menu_number_1; // �޴� ����


	vector<string> Database_1; // �޴� 1������ ����� ������ ���̽�

	do {
		printf("��¥�� �Է����ּ���: ");

		std::getline(cin, write_date);
		write_dateo = write_date;
		printf("\n");
		wrong_1 = DateCheck(&write_date); // ��¥ ���� �˻�. �����̸� 0 ���ϵǼ� ���� �޴� ���. ������ ������ 1���� ���ϵǼ� �ٽ� while�� ����
	} while (wrong_1);

	D_1.Daily_DB_Search(&Database_1, write_date, 1);  // ���� write_date�� ��¥�� �ش�Ǵ� �ϰ��� �����.

	while (true) {
		if (wrong_1) // ����� �����ϰ� �ٽ� ���ƿ�����  

		{
			system("cls");
			printf("*�Ϸ� �ϰ� �Է� �� ����*\n\n");

			printf("��¥�� �Է����ּ���: %s\n\n", write_dateo.c_str());
			D_1.Daily_DB_Search(&Database_1, write_date, 1);  // ���� write_date�� ��¥�� �ش�Ǵ� �ϰ��� �����.


		}
		else {
			
		}	

		wrong_1 = 0; // �ٽ� �ʱ�ȭ

		printf("\n�����Ͻ� ����� �������ּ���(1.���ο� Ȱ���Է� 2.���� Ȱ�� ���� 3. ���� Ȱ�� ���� ���� 4.�޴�ȭ������ ���ư���):");

		cin >> menu_number_1;

		if (menu_number_1 == "1") {
			wrong_1 = 1;
			D_1.Schedule_make(&Database_1, write_date);
		}
		else if (menu_number_1 == "2") {
			wrong_1 = 1;
			D_1.Schedule_delete(&Database_1, write_date);
		}
		else if (menu_number_1 == "3") {
			wrong_1 = 1;
			D_1.Schedule_modify(&Database_1, write_date);

		}
		else if (menu_number_1 == "4") {
			break;
		}
		else {
			printf("\n�߸��� �Է��Դϴ�. 1����4 ������ �ڿ����� �Է����ּ���\n");
		}


	}


}


int ReturnHour(int time) { // �� ��ȯ �Լ�, �Ű����� time�� �д����� �Է¹���
	int ans;

	ans = time / 60;

	return ans;
};

int ReturnMinute(int time) { // �� ��ȯ �Լ�
	int ans;

	ans = time % 60;

	return ans;

};

int Return24Rate(int time) { // 24�ð� ���� ��ȯ �Լ�
	int ans;
	int One_day = 1440;

	ans = (double)time / (double)One_day * (double)100;

	return ans;
}


//####
void DailyChart::Daily_statistics(vector<string>* db, string date_data) { // �ϰ� ��� �Լ�.

	int cat[8] = { 0, }; // �� ī�װ��� �� �ð� ����, �ʱ�ȭ ����.
	int count = 0; // ��� ��ºκ��� ó������ �ƴ��� �����ϴ� ����
	/* ���� ����

	vector<string> db : DB ���� ������ ����.

	string date_data : ó���� �Է� ���� ��¥ ���� 2020-03-20 �̷� string ���·� �޾���.

	*/

	vector<string> DB_test;

	int D_year = 0, D_month = 0, D_day = 0; // date_data���� �Է� ���� ������� �Ҵ��� ����

	All_Database.clear();

	Daily_DB_Read(&All_Database); // ���� ��ü DB ����

	stringstream stream_test;
	stream_test.str(date_data);

	stream_test >> D_year >> D_month >> D_day; // ����� ����
	stream_test.clear();



	D_year = abs(D_year);
	D_month = abs(D_month);
	D_day = abs(D_day);



	printf("********************%d�� %d�� %d���� ����� ***********************\n\n", D_year, D_month, D_day);

	Daily_DB_Search(db, date_data, 2); // �ش� ���ڸ� �����ؼ� DB ������Ʈ

	for (unsigned int i = 0; i < (*db).size(); i++) {
		stream_test.clear();
		stream_test.str((*db)[i]);

		stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
			cartegory >> detail_chart; // Ŭ���� ���� ����

		if (cartegory == "����") {
			cat[0] += btw_time;
			continue;
		}
		else if (cartegory == "�Ļ�") {
			cat[1] += btw_time;
			continue;
		}
		else if (cartegory == "����") {
			cat[2] += btw_time;
			continue;
		}
		else if (cartegory == "����") {
			cat[3] += btw_time;
			continue;
		}
		else if (cartegory == "�") {
			cat[4] += btw_time;
			continue;
		}
		else if (cartegory == "���") {
			cat[5] += btw_time;
			continue;
		}
		else if (cartegory == "�Ƹ�����Ʈ") {
			cat[6] += btw_time;
			continue;
		}
		else if (cartegory == "��Ÿ") {
			cat[7] += btw_time;
			continue;
		}
		break;
	}

	//	printf("\n\n******************************************************************************\n\n");

	for (int i = 0; i < 8; i++) {
		if (cat[i] == 0) { // �ش� ī�װ��� ������ �ð��� ������
			//�Ѿ
		}
		else {
			switch (i) {
			case 0: // 1. ����
				printf("���� : %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 1: // 2. �Ļ�
				printf("�Ļ� : %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 2: // 3. ����
				printf("���� : %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 3: // 4. ����
				printf("���� : %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 4: // 5. �
				printf("� : %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 5: // 6. ���
				printf("��� : %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 6: // 7. �Ƹ�����Ʈ
				printf("�Ƹ�����Ʈ : %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			default: // 8. ��Ÿ 
				printf("��Ÿ : %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			}
		}
	}


	printf("\n\n******************************************************************************\n\n");

	//24�ð� ���� �� Ȱ�� ���� ��� ####

	printf("24�ð����� �� Ȱ�� ���� : ");

	for (int i = 0; i < 8; i++) {
		if (cat[i] == 0) { // �ش� ī�װ��� ������ �ð��� ������
			//�Ѿ
		}
		else {

			switch (i) {
			case 0: // 1. ����
				printf("����(%d%%)", Return24Rate(cat[i]));
				break;
			case 1: // 2. �Ļ�
				if (count != 0)
					printf(", ");
				printf("�Ļ�(%d%%)", Return24Rate(cat[i]));
				break;
			case 2: // 3. ����
				if (count != 0)
					printf(", ");
				printf("����(%d%%)", Return24Rate(cat[i]));
				break;
			case 3: // 4. ����
				if (count != 0)
					printf(", ");
				printf("����(%d%%)", Return24Rate(cat[i]));
				break;
			case 4: // 5. �
				if (count != 0)
					printf(", ");
				printf("�(%d%%)", Return24Rate(cat[i]));
				break;
			case 5: // 6. ���
				if (count != 0)
					printf(", ");
				printf("���(%d%%)", Return24Rate(cat[i]));
				break;
			case 6: // 7. �Ƹ�����Ʈ
				if (count != 0)
					printf(", ");
				printf("�Ƹ�����Ʈ(%d%%)", Return24Rate(cat[i]));
				break;
			default: // 8. ��Ÿ �Ϸ� ���
				if (count != 0)
					printf(", ");
				printf("��Ÿ(%d%%)", Return24Rate(cat[i]));
				break;
			}
			count++;
		}

	}

	printf("\n\n******************************************************************************\n\n");
	printf("�ƹ� Ű�� ������ �޴�ȭ������ ���ư��ϴ�.\n");
	
	

}


void menu_2() {  //�ϰ� ���
	rewind(stdin); // �׿� Ű�� �ʱ�ȭ
	reopen++;
	system("cls");
	printf("************************************************************************************");
	printf("\n\n");
	printf("\t\t\t\t�ϰ����");
	printf("\n\n");
	printf("************************************************************************************");
	printf("\n\n");

	string Date_2;
	DailyChart D_2; // Ŭ���� ����
	int error_check; // ���� ����

	vector<string> Database_2; // �޴� 2������ ����� ������ ���̽�


	while (1) {
		printf("Insert Year/Month/Date : ");

		std::getline(cin, Date_2);

		// ��¥ ���� Ȯ��
		error_check = DateCheck(&Date_2);
		if (0 == error_check) { // ������ ������
			break;
		}
		else { //������ ������ ####
			/*�� ���� �� ���� ���� ����*/
		}

	}

	D_2.Daily_statistics(&Database_2, Date_2);



	rewind(stdin); // �׿� Ű�� �ʱ�ȭ 
	while (!kbhit()) // �ƹ�Ű �Է� ������ ����Ǵ� ��
	{

	}

}

int CheckIntercalation(int year) { // ���� ����. �����̸� 1����, �ƴϸ� 0����
	int temp;

	/* ���� ���ϴ� ��
	1. 4�� ����� �ش� �������� �Ѵ�.
	2. �׷��� 100�� ����� �ش� ���⿡�� ����.
	3. �׷��� 400�� ����� �ش� ���� �������� �ִ´�.
	4. 4000(�Ϻο��� 3200�� ����)�� ����� �ش� ������ �ƴϴ�.
	*/

	temp = year % 4;
	if (temp == 0) {
		temp = year % 100;
		if (temp == 0) {
			temp = year % 400;
			if (temp == 0) {
				return 1;
			}
			else
				return 0;
		}
		else {
			return 1;
		}
	}
	else {
		return 0;
	}
};


//####
void DailyChart::Monthly_statistics(vector<string>* db, string date_data) {  // ���� ��� �Լ�.
	int cat[8] = { 0, }; // �� ī�װ��� �� �ð� ����, �ʱ�ȭ ����.

	/* ���� ����

	vector<string> db : DB ���� ������ ����.

	string date_data : ó���� �Է� ���� ��¥ ���� 2020-03 �̷� string ���·� �޾���.

	*/


	int D_year = 0, D_month = 0; // date_data���� �Է� ���� ������� �Ҵ��� ����
	int count = 0;

	All_Database.clear();
	Daily_DB_Read(&All_Database); // ���� ��ü DB ����

	stringstream stream_test;
	stream_test.str(date_data);

	stream_test >> D_year >> D_month; // ��� ����
	stream_test.clear();

	D_year = abs(D_year);
	D_month = abs(D_month);

	vector<string> DB_test;

	printf("********************%d�� %d���� ����� ***********************\n\n", D_year, D_month);

	//db->clear();
	Daily_DB_Search(&DB_test, date_data, 0); // �ش� ���ڸ� �����ؼ� DB ������Ʈ



	// �� �ð� ���

	for (unsigned int i = 0; i < (DB_test).size(); i++) {
		stream_test.clear();
		stream_test.str((DB_test)[i]);

		stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
			cartegory >> detail_chart; // Ŭ���� ���� ����

		if (cartegory == "����") {
			cat[0] += btw_time;
			continue;
		}
		else if (cartegory == "�Ļ�") {
			cat[1] += btw_time;
			continue;
		}
		else if (cartegory == "����") {
			cat[2] += btw_time;
			continue;
		}
		else if (cartegory == "����") {
			cat[3] += btw_time;
			continue;
		}
		else if (cartegory == "�") {
			cat[4] += btw_time;
			continue;
		}
		else if (cartegory == "���") {
			cat[5] += btw_time;
			continue;
		}
		else if (cartegory == "�Ƹ�����Ʈ") {
			cat[6] += btw_time;
			continue;
		}
		else if (cartegory == "��Ÿ") {
			cat[7] += btw_time;
			continue;
		}
		break;
	}
	// �� ī�װ��� ��� �ð� ���ϱ�

	if (D_month == 1 || D_month == 3 || D_month == 5 || D_month == 7 || D_month == 8 || D_month == 10 || D_month == 12) { // 31���� �ִ� ��
		cat[0] /= 31;
		cat[1] /= 31;
		cat[2] /= 31;
		cat[3] /= 31;
		cat[4] /= 31;
		cat[5] /= 31;
		cat[6] /= 31;
		cat[7] /= 31;



	}
	else if (D_month == 2) { // 28�ϱ��� + ���� ���
		if (CheckIntercalation(D_year)) { // ������ ���
			cat[0] /= 29;
			cat[1] /= 29;
			cat[2] /= 29;
			cat[3] /= 29;
			cat[4] /= 29;
			cat[5] /= 29;
			cat[6] /= 29;
			cat[7] /= 29;
		}
		else { // �ƴ� ���
			cat[0] /= 28;
			cat[1] /= 28;
			cat[2] /= 28;
			cat[3] /= 28;
			cat[4] /= 28;
			cat[5] /= 28;
			cat[6] /= 28;
			cat[7] /= 28;

		}
	}
	else { // ������ 30�ϸ� �ִ� ��
		cat[0] /= 30;
		cat[1] /= 30;
		cat[2] /= 30;
		cat[3] /= 30;
		cat[4] /= 30;
		cat[5] /= 30;
		cat[6] /= 30;
		cat[7] /= 30;
	}


	for (int i = 0; i < 8; i++) {
		if (cat[i] == 0) { // �ش� ī�װ��� ������ �ð��� ������
			//�Ѿ
		}
		else {
			switch (i) {
			case 0: // 1. ����
				printf("��� ���� �ð�: %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 1: // 2. �Ļ�
				printf("��� �Ļ� �ð�: %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 2: // 3. ����
				printf("��� ���� �ð�: %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 3: // 4. ����
				printf("��� ���� �ð� : %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 4: // 5. �
				printf("��� � �ð�: %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 5: // 6. ���
				printf("��� ��� �ð�: %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 6: // 7. �Ƹ�����Ʈ
				printf("��� �Ƹ�����Ʈ �ð�: %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			default: // 8. ��Ÿ 
				printf("��� ��Ÿ �ð�: %d�ð� %d��\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			}
		}
	}


	printf("\n\n******************************************************************************\n\n");

	//24�ð� ���� �� Ȱ�� ���� ��� ####

	printf("24�ð����� �� Ȱ�� ���� : ");

	for (int i = 0; i < 8; i++) {
		if (cat[i] == 0) { // �ش� ī�װ��� ������ �ð��� ������
			//�Ѿ
		}
		else {

			switch (i) {
			case 0: // 1. ����
				printf("����(%d%%)", Return24Rate(cat[i]));
				break;
			case 1: // 2. �Ļ�
				if (count != 0)  // ó�� ����ϴ� �� �ƴϸ� ','�� �ʿ������� ����Ѵ�.
					printf(", ");
				printf("�Ļ�(%d%%)", Return24Rate(cat[i]));
				break;
			case 2: // 3. ����
				if (count != 0)
					printf(", ");
				printf("����(%d%%)", Return24Rate(cat[i]));
				break;
			case 3: // 4. ����
				if (count != 0)
					printf(", ");
				printf("����(%d%%)", Return24Rate(cat[i]));
				break;
			case 4: // 5. �
				if (count != 0)
					printf(", ");
				printf("�(%d%%)", Return24Rate(cat[i]));
				break;
			case 5: // 6. ���
				if (count != 0)
					printf(", ");
				printf("���(%d%%)", Return24Rate(cat[i]));
				break;
			case 6: // 7. �Ƹ�����Ʈ
				if (count != 0)
					printf(", ");
				printf("�Ƹ�����Ʈ(%d%%)", Return24Rate(cat[i]));
				break;
			default: // 8. ��Ÿ 
				if (count != 0)
					printf(", ");
				printf("��Ÿ(%d%%)", Return24Rate(cat[i]));
				break;
			}
			count++;
		}

	}

	printf("\n\n******************************************************************************\n\n");
	printf("�ƹ� Ű�� ������ �޴�ȭ������ ���ư��ϴ�.\n");

	
}


//#### (���Ʊ�) cf) (��)insert_month
int DateCheck2(string* search_date) { // �⵵, �� �� ������ ��������
	/* date �� ǥ�� ���·� �ٲٴ� ������ ���Խ�ų ��*/
	//2020/100 , 2020/001 , 001, �� �⵵o �� �⵵x ��  1,3,5,6
	int o1, o2, o3, o4, z1,z2,z3,z4,z5 ,si, si2,l;
	
	si = sign((char*)(*search_date).c_str());
	si2 = signmiss((char*)(*search_date).c_str());
	l = last((char*)(*search_date).c_str());
	z5 = zerogu4((char*)(*search_date).c_str()); // �Ǿ� 
	z1= zerogu1((char*)(*search_date).c_str());
	z2 = zerogu5((char*)(*search_date).c_str());


	if (si == 2) {
		printf("�߸��� ���ڸ� �Է��ϼ̽��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
		return 1;
	}
	if ((*search_date)[0] == '.' || (*search_date)[0] == '/' || (*search_date)[0] == ' ' || (*search_date)[0] == '-') {
		printf("���Ŀ� ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
		return 1;
	}
	if (si2 == 2) {
		printf("���ڿ��� �ѹ��� �� �� �Է��߽��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
		return 1;
	}
	if (l == 2) {
		printf("���Ŀ� ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
		return 1;
	}


	char* oper = strchr((char*)search_date->c_str(), '.');
	char* oper1 = strchr((char*)search_date->c_str(), '/');
	char* oper2 = strchr((char*)search_date->c_str(), ' ');
	char* oper3 = strchr((char*)search_date->c_str(), '-'); //��͵� ã�Ƽ� ��ġ��ȯ
	char* num = strtok((char*)search_date->c_str(), " -/.");

	int nums[100];
	int i = 0; ; int j = 0;
	char nums2[100];

	if (oper != NULL || oper1 != NULL || oper2 != NULL || oper3 != NULL) { // ���й��ڰ� �ִ��� ������ ���� ����
	  //���й��� ���� ���� ����
		while (num != NULL) { //�迭�� ���ڵ� ����
			nums[i++] = atoi(num);
			num = strtok(NULL, " -/.");
		}//  �⵵���� i�� 3��ȯ 4������ �߸���
		if (i > 2) {
			printf("���ڿ��� �ùٸ��� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
			return 1;
		}
		else {
			if (i == 2) {
				if (nums[0] >= 2000 && nums[0] <= 2999) {
					if (nums[1] >= 1 && nums[1] <= 12) {
						//printf("��¥ �ֱ�1");
						if (z1 == 2) {
							printf("���๮�� ��0���� �ʹ� ���� �Է��߽��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
							return 1;
						}
						string y = to_string(nums[0]);
						string m = to_string(nums[1]);
						*search_date = y + '-' + m;
						return 0;
					}
					else {
						printf("���� �Է¹����� �߸��Ǿ����ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
						return 1;
					}
				}
				else {
					printf("�⵵ ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
					return 1;
				}
			}//�⵵ ���� ���Է��Ѱ�
			else {
				printf("���Ŀ� ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
				return 1;
			}//���̳� �����Է��Ѱ� , ���й��� �������
		}// printf("���й�������");
	}// ���й����ִ°� ����
	else {// printf("���й��ھ���");
	   //���� �⵵�Է�, �⵵ ���Է����� ���� 4�ڸ� 2�ڸ� 2�ڸ�
		int i = 0;
		int num = strlen(search_date->c_str());
		if (num == 6) {
			//year1,month1,date1
			nums[0] = year1((char*)search_date->c_str()); nums[1] = month1((char*)search_date->c_str());
			if ((nums[0] >= 2000 && nums[0] <= 2999)) {
				if (nums[1] >= 1 && nums[1] <= 12) {
					//printf("��¥�Է�3");
					string y = to_string(nums[0]);
					string m = to_string(nums[1]);
					*search_date = y + '-' + m;
					return 0;
				}
				else {
					printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
					return 1;
				}
			}
			else {
				printf("�⵵ ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
				return 1;
			}
		}
		else if (num == 2) { // month2 , date 2
			time_t base_time = time(NULL);
			struct tm* newtime = localtime(&base_time);
			int t = newtime->tm_year + 1900;
			nums[0] = month2((char*)search_date->c_str());
			if (nums[0] >= 1 && nums[0] <= 12) {
				//printf("��¥�ֱ�4");
				string y = to_string(t);
				string m = to_string(nums[0]);
				*search_date = y + '-' + m;
				return 0;
			}
			else {
				printf("�� ������ ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���.\n");
				return 1;
			}
		}
		else {
			printf("���Ŀ� ���� �ʽ��ϴ�. ��¥�� �ٽ� �Է����ּ���. \n");
			return 1;
		}
	}

	return 0;
}


void menu_3() { // ���� ���
	rewind(stdin); // �׿� Ű�� �ʱ�ȭ
	reopen++;
	system("cls");
	printf("************************************************************************************");
	printf("\n\n");
	printf("\t\t\t*���� ���*");
	printf("\n\n");
	printf("************************************************************************************");
	printf("\n\n");

	string Date_3;
	DailyChart D_3; // Ŭ���� ����
	int error_check; // ���� ����

	vector<string> Database_3; // �޴� 2������ ����� ������ ���̽�


	while (1) {
		printf("Insert Year/Month: ");

		std::getline(cin, Date_3);

		// ��¥ ���� Ȯ��
		error_check = DateCheck2(&Date_3);
		if (0 == error_check) // ������ ������
		{
			break;
		}
		else //������ ������ ####
		{
			/*�� ���� �� ���� ���� ����*/
		}

	}

	D_3.Monthly_statistics(&Database_3, Date_3);



	rewind(stdin); // �׿� Ű�� �ʱ�ȭ 
	while (!kbhit()) // �ƹ�Ű �Է� ������ ����Ǵ� ��
	{

	}

}

void menu_4() { // ���� (����ȣ)
	rewind(stdin); // �׿� Ű�� �ʱ�ȭ
	system("cls");
	reopen++;
	vector<string> help_DB;
	//DailyChart D_4;

	ifstream readfile; // �о���� ���� ����



	readfile.open("Help.txt");


	if (readfile.is_open()) {
		while (!readfile.eof())
		{
			string str;
			std::getline(readfile, str); // DB.txt ������ ���پ� �о��


			(help_DB).push_back(str); // �о�� ������ db �������ڿ� �������� ������.

		}
	}

	readfile.close();

	for (unsigned int i = 0; i < (help_DB).size(); i++) {// db ũ�� ��ŭ ��� ���� �����.
		cout << (help_DB)[i] << endl;

	}

	
	rewind(stdin); // �׿� Ű�� �ʱ�ȭ 
	while (!kbhit()) // �ƹ�Ű �Է� ������ ����Ǵ� ��
	{

	}
	//getch(); // �ƹ� Ű �Է��ϸ� �����.
}

void menu_5() {// ���� (���ؿ�)
	rewind(stdin); // �׿� Ű�� �ʱ�ȭ
	system("cls");
	printf("******************************************************************************\n\n");
	printf("\n\n\n");
	printf("\t\t\t\tBye\n");
	printf("\n\n");
	printf("\n\n******************************************************************************\n\n");
	printf("�� â�� �������� �ƹ� Ű�� ��������...\n");

	rewind(stdin); // �׿� Ű�� �ʱ�ȭ 

	while (!kbhit()) // �ƹ�Ű �Է� ������ ����Ǵ� ��
	{

	}
	
	
}


int main(void) {


	DailyChart D; // Ŭ���� ����


	string want_data; // (ã�� ���� ������) �Է� ���� string ����

	All_Database.clear(); // ���� �迭 �ʱ�ȭ



	string menu_number;


	// DB.txt üũ (����ȣ)
	int n;
	string err_text;
	if (db_check == 0)
	{
		if (D.Daily_DB_Check(&err_text, &n)) { // �츮�� DB�� �ƴϸ� 1�� ��ȯ�Ǿ if���� �����.
			printf("DB�� (%s/�Է¾��)�� ���� �ʾ� �ջ�Ǿ����ϴ�. DB.txt�� (%d ��)�� �����Ͻʽÿ�.\n", err_text.c_str(), n);
			Sleep(2000); // DB�� �������϶� ��µǴ� ������ �����ַ��� ����
			return 0;
		}
		else { // �츮 DB�� ������ 0�� ��ȯ�Ǿ else���� �����.
			printf("DB�� �����Դϴ�!\n");

		}

		Sleep(2000); // DB�� �����϶� ��µǴ� ������ �����ַ��� ����

		// DB. txt ���� ���� ������� (�ʱ� DB ����)

		All_Database.clear();

		D.Daily_DB_Read(&All_Database); // All_Database ���� �迭���ٰ� �о�� DB ������ ����.
	
		db_check++;
	}

	char menu_num[1000];
	
	// ���� ȭ�� (���ؿ�)


	while (1) {
		rewind(stdin); // �׿� Ű�� �ʱ�ȭ : ���־� ��Ʃ��������� �����Ǵ� �Լ��� �ٸ� ������ ������.(����)
		system("cls");
		printf("******************************************************************************\n\n");
		printf("Hello. We are Team2\n\n");
		printf("Captain : ������\n");
		printf("Member : ����ȣ, ���Ʊ�, ������, ���ؿ�\n");
		printf("Subject : �������� ������Ʈ1\n");
		printf("Professor : ������\n");
		printf("\n\n******************************************************************************\n\n");
		printf("Please Select Menu Number\n\n");
		printf("1. �����Է�, ����, ����\n");
		printf("2. ī�װ� �߰�, ����, ����\n");
		printf("3. �ϰ����\n");
		printf("4. �������\n");
		printf("5. �����\n");
		printf("6. ����\n");

		while (1) {
			
			printf("Insert Number : ");

			
			//gets_s(menu_num);

			//menu_number = menu_num;

			//cin.ignore();
			//getline(cin, menu_number);

			cin >>menu_number;

			//rewind(stdin); // �׿� Ű�� �ʱ�ȭ 
			//kbhit();// getchar();
			/*
			if (menu_number.length() > 1 || menu_number.length() == 0)
			{
				printf("\n�߸��� �Է��Դϴ�. 1����5 ������ �ڿ����� �Է����ּ���\n");
				rewind(stdin); // �׿� Ű�� �ʱ�ȭ 
				while (!kbhit()) // �ƹ�Ű �Է� ������ ����Ǵ� ��
				{

				}
				rewind(stdin); // �׿� Ű�� �ʱ�ȭ 
				//cin.ignore();
				break;
			}*/


			/* cf) C++�� String Ÿ�� ���� C����� �迭�θ� �ް� �Ͷ�

			���� :

				  ���� cpp�� string ����� ���� �ʰ� C���θ� ����� ���� ��

				  �迭 Ÿ������ �޾Ƶ��̷���

				  char* arr[] = new char[]; �̷������� �����Ҵ� ����. (�޸� ���� ����, �������� ũ�⸦ �޾Ƶ��̱� ����)

				  scanf�� arr�� �Է��� ���� ����

				  arr[1] ������ �񱳸� �ؼ�

				  (arr[0]�� �ִ� = 1���ڸ���
				  arr[1]�� �ִ�? = �ڸ����� 2�� �̻����� �̿�)

				  ������ �� �ڸ��� �̻� ���ڸ� �Է� �����Ŷ� else (����ó��)�ϰ�, ������

				  ���ڸ����� �ִ� ���̶� arr[0] == '1','2','3','4','5'' else �̷��� ó���ϸ� ��.

				  ���� �� ��.

			 */


			if (menu_number == "1") { // ���� �Է�, ����, ���� �޴�

				menu_1();
				break;
			}
			else if (menu_number == "2") { // �ϰ� ��� �޴�

				menu_2();
				//cin.ignore();
				break;
			}
			else if (menu_number == "3") { // ���� ��� �޴�

				menu_3();
				
				break;
			}
			else if (menu_number == "4") { // ����� �޴�

				menu_4();
				break;
			}
			else if (menu_number == "5") { // �����ϱ�

				menu_5();
				exit(0);

			}
			else {
			/*if (reopen == 1){
					reopen--;
				}
				else {
					
					Sleep(1000);
				}
			    getch(); // �ƹ� Ű �Է��ϸ� �����.
				main();*/

				printf("\n�߸��� �Է��Դϴ�. 1����5 ������ �ڿ����� �Է����ּ���\n");
				while (!kbhit()) // �ƹ�Ű �Է� ������ ����Ǵ� ��
				{

				}
				rewind(stdin); // �׿� Ű�� �ʱ�ȭ 

				//getch(); // �ƹ� Ű �Է��ϸ� �����.
				break;
			}
		}
	}



	return 0;
}