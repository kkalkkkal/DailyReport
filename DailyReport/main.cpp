#include <iostream> // I/O stream : C++에서 대충 stdio.h에 해당
#include <fstream> // 파일 입출력
#include <string> // string 타입 사용
#include <vector> // 벡터 타입 사용, string 타입의 배열 사용 가능해짐
#include <sstream> // 문자열에서 필요한 정보를 빼낼때 사용
#include <cstdlib> // int형 절대값 함수 abs와 타입 변환 함수 atoi 사용하려고 선언
#include <time.h>
#include <cctype>
#include <windows.h>
#include <conio.h>

using namespace std;
#pragma warning(disable:4996) // _s 시리즈 함수만 사용하게끔 디버깅 안되서 이를 무시함.
int db_check=0;
int reopen = 0;
/*
tip) 출력할때 되도록 printf를 쓰는 이유 :
'cout << endl;' 구문은 printf 보다 메모리를 훨씬 많이 먹기 때문이다.
최적화를 위해선 cout 보단 printf를 쓰는 것이 좋다.

그럼에도 가끔 cout을 쓰는 이유 : 직관적이고 쓰기 편하다.

*/


class DailyChart { // 일정 관련 처리를 모두 이 클래스에서 처리할 것임. (강민호)
private:
	int num = 0; // 고유번호
	int year = 0; // 년
	int month = 0; // 월
	int day = 0; //일
	string start_time; // 시작 시간
	string end_time; // 종료 시간
	int btw_time = 0; // 시간 차이
	string cartegory; // 카테고리
	string detail_chart; // 세부사항
public:
	int Daily_DB_Check(string *, int*);
	void Daily_DB_Read(vector<string>*);   //  포인터로 넘겨 받아야 메인의 All_Database에 정보가 모두 저장이 된다.
	void Daily_DB_Search(vector<string>*, string, int); // 해당 일자 검색 함수
	void Schedule_make(vector<string>*, string); // 일정 입력 함수
	void Schedule_modify(vector<string>*, string); // 일정 수정 함수
	void Schedule_delete(vector<string>*, string); // 일정 삭제 함수
	int CheckP_ID(vector<string>*, string); // 고유번호 확인
	void Daily_statistics(vector<string>*, string); // 일간 통계
	void Monthly_statistics(vector<string>*, string); // 월간 통계
};

vector<string> All_Database; // 앞으로 사용할 전체 DB 벡터 전역 변수

void menu_1();  // 메뉴1번 일정입력/수정/삭제 탭이다
void menu_2();  // 메뉴2번 일간통계 탭이다
void menu_3();  // 메뉴3번 월간통계 탭이다
void menu_4();  // 메뉴4번 사용방법 탭이다
void menu_5();  // 메뉴5번 종료탭이다


int DailyChart::Daily_DB_Check(string *err_text, int* a) { // DB 체크 함수 (강민호)

	ifstream CheckFile("DB.txt");


	int count = 0, i = 0, length = 0, check_id = 0;
	int count2 = 0; //각 인자가 없는 경우를 재기 위한 카운터 변수

	// 각 테스트할 에러 인자 선언
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
	

	if (CheckFile.is_open()) { // DB.txt 파일이 있으면

		//진짜 해당 프로그램에서 필요한 DB인지 형식 확인


		while (!CheckFile.eof()) { // DB가 끝날때까지

			string tmp;
			std::getline(CheckFile, tmp); // 한줄씩 읽어옴.

			sstream_test.clear();
			sstream_test.str(tmp);
			
			// 각 인자 초기화
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
				cartegory_err >> detail_chart_err; // 클래스 인자 구분

			count = 0;
			i = 0;
			check_id = atoi(num_err.c_str());

			length = tmp.length();

			if (length == 0){ // 길이가 0이란 것은 끝났거나 중간에 엔터가 있단 뜻이다.
				if (CheckFile.eof()){ // 만일 정말로 끝나있으면
					CheckFile.close();
					break; // 종료
				}
				else { // 파일이 끝나있지 않으면 (엔터때문에 뒤가 이어져있다면)
					// 오류출력
					CheckFile.close();
					*a = check_id; // 몇 번째 고유번호에서 오류 났는지 반환
					*err_text = "속성";
					return 1;
				}


			}

			while (i <= length) {
				if (tmp[i] == '\t') { //1. 탭의 개수를 확인하여 속성 개수를 확인.

					count++;
				}

				i++;

			}

			if (count == 8) { // 속성의 개수가 9개로 일치함. (\t은 각 항목 사이에만 존재하므로 8개가 맞음.)

			}
			else { // 개수가 일치하지 않음. 일단 우리의 DB가 아님.
				CheckFile.close();
				*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
				*err_text = "속성";
				return 1;
			}

			/*
			@ 먼저 각 인자별 없는 부분을 확인
			 point ) 탭으로 전부 구분이 되어있다는 것을 확인했으니 
			 해당 행은 (고유번호)\t(년도)\t(월)\t(일)\t(시작시간)\t(끝난시간)\t(시간차)\t(카테고리)\t(세부사항) 으로 작성되어있다.
			 각 행이 없을경우

			//\t(년도)\t(월)\t(일)\t(시작시간)\t(끝난시간)\t(시간차)\t(카테고리)\t(세부사항)
			//(고유번호)\t\t(월)\t(일)\t(시작시간)\t(끝난시간)\t(시간차)\t(카테고리)\t(세부사항)
			//(고유번호)\t(년도)\t\t(일)\t(시작시간)\t(끝난시간)\t(시간차)\t(카테고리)\t(세부사항)
			//(고유번호)\t(년도)\t(월)\t\t(시작시간)\t(끝난시간)\t(시간차)\t(카테고리)\t(세부사항)
			//(고유번호)\t(년도)\t(월)\t(일)\t\t(끝난시간)\t(시간차)\t(카테고리)\t(세부사항)
			//(고유번호)\t(년도)\t(월)\t(일)\t(시작시간)\t\t(시간차)\t(카테고리)\t(세부사항)
			//(고유번호)\t(년도)\t(월)\t(일)\t(시작시간)\t(끝난시간)\t\t(카테고리)\t(세부사항)
			//(고유번호)\t(년도)\t(월)\t(일)\t(시작시간)\t(끝난시간)\t(시간차)\t\t(세부사항)

			위와 같음을 알 수 있다.

			즉, 처음으로 탭이 연속으로 두번 겹치는 순간을 잘 재면 되는 것이다.
			여기서 문제는 고유번호를 구분하는 것이다.
			*/

			count2 = 0;

			for (i = 0; i < length - 1; i++)
			{
				if (tmp[0] == '\t') // 고유번호 인자가 없을 때
				{
					CheckFile.close();
					*a = NULL; // 고유번호가 없으니 NULL 반환
					*err_text = "고유번호";
					return 1;
				}

				if (tmp[i] == '\t')
					count2++; // 탭 개수 카운트

				if (tmp[i] == '\t' && tmp[i + 1] == '\t') // 바로 뒤에 연속으로 나오면?
				{
					if (count2 == 1)
					{
						CheckFile.close();
						*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
						*err_text = "년도";
						return 1;
					}
					else if (count2 == 2)
					{
						CheckFile.close();
						*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
						*err_text = "월";
						return 1;
					}
					else if (count2 == 3)
					{
						CheckFile.close();
						*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
						*err_text = "일";
						return 1;
					}
					else if (count2 == 4)
					{
						CheckFile.close();
						*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
						*err_text = "시작시간";
						return 1;
					}
					else if (count2 == 5)
					{
						CheckFile.close();
						*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
						*err_text = "마감시간";
						return 1;
					}
					else if (count2 == 6)
					{
						CheckFile.close();
						*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
						*err_text = "시간차";
						return 1;
					}
					else if (count2 == 7)
					{
						CheckFile.close();
						*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
						*err_text = "카테고리";
						return 1;
					}
				}
			}


			/*
			(고유번호)\t(년도)\t(월)\t(일)\t(시작시간)\t(끝난시간)\t(시간차)\t(카테고리)\t <- 세부사항만 빠진 경우는 아래 과정에서 잘 걸리지므로 제외.
			*/
			
			
			//2. 각 속성의 형식을 확인

			// 고유번호 확인
			if (atoi(num_err.c_str()) < 0){
				CheckFile.close();
				*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
				*err_text = "고유번호";
				return 1;
			}

			// 연도 확인
			if (atoi(year_err.c_str()) < 2000 || atoi(year_err.c_str()) > 2999){
				if (check_id == 0) { // 처음 DB.txt 파일을 생성할때 첫 고유번호가 0이므로 이는 제외함.

				}
				else {
					CheckFile.close();
					*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
					*err_text = "연도";
					return 1;
				}

			}

			// 월 확인
			if (atoi(month_err.c_str()) < 1 || atoi(month_err.c_str()) > 12){
				if (check_id == 0) { // 처음 DB.txt 파일을 생성할때 첫 고유번호가 0이므로 이는 제외함.

				}
				else {
					CheckFile.close();
					*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
					*err_text = "월";
					return 1;
				}
			}

			// 일 확인
			if (atoi(day_err.c_str()) < 1 || atoi(day_err.c_str()) > 31){
				if (check_id == 0){ // 처음 DB.txt 파일을 생성할때 첫 고유번호가 0이므로 이는 제외함.

				}
				else {
					CheckFile.close();
					*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
					*err_text = "일";
					return 1;
				}
			}

			//시간 확인

			if (atoi(start_time_err.c_str()) < 0 || atoi(start_time_err.c_str()) > 2400){
				CheckFile.close();
				*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
				*err_text = "시작시간";
				return 1;
			}

			if (atoi(end_time_err.c_str()) < 0 || atoi(end_time_err.c_str()) > 2400){
				CheckFile.close();
				*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
				*err_text = "마감시간";
				return 1;
			}

			// 시간차 확인

			if (atoi(btw_time_err.c_str()) < 0 || atoi(btw_time_err.c_str()) > 1440){
				CheckFile.close();
				*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
				*err_text = "시간차";
				return 1;
			}



			//카테고리 확인
			if (cartegory_err == "수면" || cartegory_err == "식사" || cartegory_err == "수업" || cartegory_err == "공부" || cartegory_err == "운동" || cartegory_err == "취미" || cartegory_err == "아르바이트" || cartegory_err == "기타"){

			}
			else if (check_id == 0) { // 처음 DB.txt 파일을 생성할때 첫 고유번호가 0이므로 이는 제외함.

			}
			else {
				CheckFile.close();
				*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
				*err_text = "카테고리";
				return 1;
			}

			// 세부내용 확인
			int n_byte = strlen(detail_chart_err.c_str());
			if (n_byte <= 0 || n_byte > 100){
				CheckFile.close();
				*a = check_id; // 몇 번째 줄에서 오류 났는지 반환
				*err_text = "세부내용";
				return 1;

			}

		}




	}
	else { // DB.txt 파일이 없으면 만듦

		CheckFile.close();

		ofstream GenerateFile;
		GenerateFile.open("DB.txt"); // 파일 생성
		GenerateFile.close(); // 파일 닫기


		// 파일 쓰기

		ofstream WriteFile("DB.txt", ios::app);

		string str = "0\t0\t0\t0\t0\t0\t0\t0\t0\n";

		WriteFile << str;

		WriteFile.close();


		return 0;
	}
	return 0;
}

void DailyChart::Daily_DB_Read(vector<string>* db) { // DB 읽어와서 반환하는 함수 (강민호)

	ifstream readfile; // 읽어들일 파일 변수



	readfile.open("DB.txt");


	if (readfile.is_open()) {
		while (!readfile.eof()) {
			string str;
			std::getline(readfile, str); // DB.txt 파일을 한줄씩 읽어옴


			(*db).push_back(str); // 읽어온 정보를 db 벡터인자에 차곡차곡 저장함.

		}
		
	}



	readfile.close();

}


void DailyChart::Daily_DB_Search(vector<string>* db, string date_data, int option) { // DB에서 찾을 내용 반환하는 함수 (강민호)

	/* 인자 설명

	vector<string> db : DB 정보 포인터 인자.

	string date_data : 처음에 입력 받을 짜정보 2020-03-20 이런 string 형태로 받았음.

	*/

	int D_year = 0, D_month = 0, D_day = 0; // date_data에서 입력 받은 년월일을 할당할 변수

	All_Database.clear(); //
	db->clear();
	Daily_DB_Read(&All_Database); // 현재 전체 DB 갱신
	

	int index_count = 0;

	vector<string> DB_test; // 서치 안에서 사용할 함수

	/* 참고 사항

	현 함수는 기본적으로 2020-03-20 같은 형태로 년월일이 구분가능한 인자로
	string을 받아들였다는 전제로 제작되었다.

	예외로 인한 오류 같은 건 나중에 수정할 예정.

	*/

	stringstream stream_test;
	stream_test.str(date_data);

	stream_test >> D_year >> D_month >> D_day; // 년월일 구분
	stream_test.clear();


	/* 년 월일 구분법

	stringstream에 의해 int 인자인 year, month, day에 순서대로 int값만 차곡차곡 저장함
	2020-03-20 의 경우  2020, -03, -20으로 받아들여서 절대값 씌움.
	그럼, 2020년 3월 20일로 분류할 수 있음.

	2020_03_20 이나 '2020 03 20' 의 경우 수학기호가 아닌 밑첨자나 공백이라서
	알아서 2020년 3월 20일로 분류됨

	따라서 입력 단계에서 년, 월, 일의 형식만 제대로 입력 받으면
	아무렇게나도 알아서 분류된다.

	####
	-> 이런 분류법의 단점 : 20200203 같이 구분자 없이 입력 받으면 분류가 안됨.
							이런 경우 구분자를 만들어줘서 다시 돌려줘야하는 수고로운 코드를 만들어야하지만 일단 보류

							+) 2020.07.20 의 경우, double 형으로 인식이 되어서 아예 안돌아간다...
							이 경우의 예외처리도 잘 생각해보자

	*/

	D_year = abs(D_year);
	D_month = abs(D_month);
	D_day = abs(D_day);

	int size = (All_Database).size();  //db 크기 디버깅 하려구 만든거

	if (option == 1) {
		printf("********************%d년 %d월 %d일의 일과 ***********************\n\n", D_year, D_month, D_day);
		printf("고유번호\t시작시간\t마감시간\t활동\t\t세부내용\n\n");
	}

	for (int i = 0; i < All_Database.size() - 1; i++) { // db 크기 만큼 모든 행을 조사함.

		// 각 행마다 일치하는 날짜에 대한 정보가 있는지 조사!


		/*
		현재 DB는 db[0] = "1\t2020\t03\t20\t0600\t0900\t180\t식사\t아침식사"
		이런 식으로 저장이 되어있다.

		테이블 형식이 순서대로 int, int, int, int, string, string, int, string, string 이므로

		이 순서대로 sstream을 이용해 인자를 받는다.

		여기서 vector은 string 형이기에 db[i]의 모든 내용은 다 string 형인 점을 기억하자.
		*/


		stream_test.clear(); // sstream은 한번 쓰고 나면 다시 초기화 시켜야 쓸 수 있다.

		stream_test.str((All_Database)[i]);

		stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
			cartegory >> detail_chart; // 클래스 인자 구분


		if (D_year == year && D_month == month) {

			if (option && D_day == day) { // 날짜 정보가 일치하면
				
				(*db).push_back((All_Database)[i]); // 읽어온 정보를 db 벡터인자에 차곡차곡 저장함.	
				DB_test.push_back((All_Database)[i]);
				index_count++;
			}
			else if(option == 2 && D_day == day)
			{
				(*db).push_back((All_Database)[i]); // 읽어온 정보를 db 벡터인자에 차곡차곡 저장함.	
				DB_test.push_back((All_Database)[i]);
				index_count++;
			}
			else if (!option) { // 년월만 맞는지 확인해서 (월간 출력)
				(*db).push_back((All_Database)[i]); // 읽어온 정보를 db 벡터인자에 차곡차곡 저장함.
				DB_test.push_back((All_Database)[i]);
				index_count++;
			}


		}



	}

	//시간순서대로 저장된 일과 출력(이정수)


	int num_j = 0; // 고유번호
	int year_j = 0; // 년
	int month_j = 0; // 월
	int day_j = 0; //일
	int start_time_i;
	int start_time_j; // 시작 시간
	int tmp;
	int k=0, count_k=0;
	string k_test;

	
	if (index_count == 0){
		printf("***************************************************************\n");
	}
	else if (option == 1) {



		for (size_t i = 0; i < DB_test.size() - 1; i++) { // 일과 정보 선택 정렬 출력
			stream_test.clear();
			string temp;
			tmp = i;
			stream_test.str(DB_test[i]);
			stream_test >> num >> year >> month >> day >> start_time_i; // 클래스 인자 구분


			for (size_t j = i + 1; j < DB_test.size(); j++) {

				stream_test.clear();
				stream_test.str(DB_test[j]);

				stream_test >> num_j >> year_j >> month_j >> day_j >> start_time_j; // 클래스 인자 구분

				if (start_time_i > start_time_j) {

					temp = DB_test[i];
					DB_test[i] = DB_test[j];
					DB_test[j] = temp;

					stream_test.clear(); // 자리를 바꿨으니 다시 한번 갱신함
					stream_test.str(DB_test[i]);
					stream_test >> num >> year >> month >> day >> start_time_i; // 클래스 인자 구분

				}

			}

		}


		//여기에 출력
		for (size_t i = 0; i < DB_test.size(); i++) {
			k_test = DB_test[i];

			stream_test.clear();
			stream_test.str(DB_test[i]);
			stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
				cartegory >> detail_chart; // 클래스 인자 구분		

			
			count_k = 0;

			//#### 세부사항 스페이스바까지 고려함.
			for (k = 0; k < k_test.length() ; k++)
			{
				if (k_test[k] == '\t')
				{
					count_k++;
				}

				if (count_k == 8) // 다음 속성이 세부사항일때
				{
					detail_chart = k_test.substr(k+1, k_test.length() - k -1 ); // 탭이 있던 자리 다음부터 끝까지 복사
					break;
				}
			}

			if (cartegory == "아르바이트"){
				cout << num << "\t\t" << start_time.substr(0, 2) + ":" + start_time.substr(start_time.length() - 2, 2) << "\t\t" << end_time.substr(0, 2) + ":" + end_time.substr(start_time.length() - 2, 2) << "\t\t" << cartegory << "\t" << detail_chart << '\n' << endl; // 해당 정보 전부 출력
			}
			else {
				cout << num << "\t\t" << start_time.substr(0, 2) + ":" + start_time.substr(start_time.length() - 2, 2) << "\t\t" << end_time.substr(0, 2) + ":" + end_time.substr(start_time.length() - 2, 2) << "\t\t" << cartegory << "\t\t" << detail_chart << '\n' << endl; // 해당 정보 전부 출력
			}

		}

		printf("***************************************************************\n");
	}

}


int ReturnBtwTime(int start_minute, int end_minute, int start_hour, int end_hour) { // 시간차 구해주는 함수 (강민호)


	int Time;

	// 분단위로 환산해서 리턴

	if (end_minute - start_minute < 0) { // 분끼리 뺐을 때 음수가 나올때 ; 예) 0149 ~ 0239 의 경우

		Time = ((end_hour - 1) - start_hour) * 60
			+ (end_minute + 60) - start_minute;
	}
	else { // 말끔히 계산할 수 있는 경우

		Time = (end_hour - start_hour) * 60 + end_minute - start_minute;
	}



	return Time;
};



int CheckDetail(string* detail) { // 세부사항 오류 체크 함수 (강민호)

	/* 탭을 스페이스로 변환시켜주고, 100byte를 넘기지 않았나 체크*/

	// 표준화

	for (int i = 0; i < detail->size(); i++) {

		if ((*detail)[i] == '\t') { // 탭('\t') 공백(' ') 처리

			(*detail)[i] = ' ';
		}
	}

	
	int n_byte = strlen(detail->c_str()); // byte 반환


	if (n_byte <= 100 && n_byte >= 1) {
		return 1;
	}
	else if (n_byte == 0){
		printf("반드시 1자 이상 써야합니다.\n");
		return 0;
	}
	else {
		printf("[%d]byte로 입력할 수 있는 문자열을 초과했습니다. 100byte까지 입력 가능합니다.\n", n_byte);
		return 0;
	}

};




//날짜용 예외처리 함수들(김훈기)  
int onjum(char a[]); //구분문자 온점 입력시 온점으로 통일한지 확인하는 함수 
int slash(char a[]); //구분문자 슬래쉬 입력시 슬래쉬로 통일한지 확인하는 함수
int space(char a[]); //구분문자 공백 입력시 공백으로 통일한지 확인하는 함수
int minus1(char a[]);//구분문자 마이너스 입력시 마이너스로 통일한지 확인하는 함수
int year1(char a[]); // 구분문자 없는경우 년도를 배열에 넣어주는 함수
int month1(char a[]); // 구분문자 없는경우 월을 배열에 넣어주는 함수
int date1(char a[]); // 구분문자 없는경우 일을 배열에 넣어주는 함수
int date2(char a[]); // 구분문자 없는경우 일을 배열에 넣어주는 함수
int month2(char a[]); // 구분문자 없는경우 월을 배열에 넣어주는 함수
int zerogu1(char a[]); //선행문자 '0'이 두개이상 있는지 확인하는 함수
int zerogu3(char a[]); //선행문자 '0'이 두개이상 있는지 확인하는 함수
int zerogu5(char a[]); //선행문자 '0'이 두개이상 있는지 확인하는 함수
int zerogu6(char a[]); //선행문자 '0'이 두개이상 있는지 확인하는 함수
int sign(char a[]); //잘못된 문자가 있는지 확인하는 함수
int signmiss(char a[]); // 구분문자가 연속으로 나오는지 확인하는 함수
int zerogu4(char a[]); //년도에 선행 '0'
int last(char a[]); // 마지막에 구분문자 

//(김훈기) cf) (구) insert_date 함수
int DateCheck(string* search_date) { // 날짜 오류 체크 함수 : 정상이면 0리턴 오류면 1리턴



	//공백, - , / , . 으로 구분가능
	// 2000- 2999 년도
	// 01 ,1  001(x)

	 //만약 " -/."가 구분문자가 ㅇ아니라면 오류 

	int o1, o2, o3, o4, z1,z2,z3,z4, si, si2, z5,l;
	o1 = slash((char*)search_date->c_str());
	o2 = onjum((char*)search_date->c_str());
	o3 = space((char*)search_date->c_str());
	o4 = minus1((char*)search_date->c_str());
	z1 = zerogu1((char*)search_date->c_str()); // o o 월
	z2 = zerogu3((char*)search_date->c_str());// o o 일
	z3 = zerogu5((char*)search_date->c_str());// o x 월
	z4 = zerogu6((char*)search_date->c_str());// o x 일
	si = sign((char*)search_date->c_str());
	si2 = signmiss((char*)search_date->c_str());
	z5 = zerogu4((char*)search_date->c_str());
	l = last((char*)search_date->c_str());

	if (si == 2) {
		printf("잘못된 문자가 있습니다. 날짜를 다시 입력해주세요. \n");
		return 1;
	}
	if ((*search_date)[0] == '.' || (*search_date)[0] == '/' || (*search_date)[0] == ' ' || (*search_date)[0] == '-') {
		printf("형식에 맞지 않습니다. 날짜를 다시 입력해주세요. \n");
		return 1;
	}
	if (si2 == 2) {
		printf("문자열을 한번에 두 번 이상 입력했습니다. 날짜를 다시 입력해주세요. \n");
		return 1;
	}
	if (l == 2) {
		printf("형식에 맞지 않습니다. 날짜를 다시 입력해주세요. \n");
		return 1;
	}

	char* oper = strchr((char*)search_date->c_str(), '.');
	char* oper1 = strchr((char*)search_date->c_str(), '/');
	char* oper2 = strchr((char*)search_date->c_str(), ' ');
	char* oper3 = strchr((char*)search_date->c_str(), '-'); //요것들 찾아서 위치반환
	char* num = strtok((char*)search_date->c_str(), " -/.");

	int nums[100];
	int i = 0; ; int j = 0;
	char nums2[100];

	if (oper != NULL || oper1 != NULL || oper2 != NULL || oper3 != NULL) { // 구분문자가 있는지 없는지 먼저 구분
		//구분문자 빼고 숫자 저장
		while (num != NULL) { //배열에 숫자들 저장
			nums[i++] = atoi(num);
			num = strtok(NULL, " -/.");
		}//  년도까지 i는 3반환 4개부턴 잘못쓰
		if (i > 3) {
			printf("문자열이 올바르지 않습니다. 날짜를 다시 입력해주세요.\n");
			return 1;
		}
		else {
			if (i == 3) {
				if (o1 == 1 || o2 == 1 || o3 == 1 || o4 == 1) {
					if (z5 == 2) {
						printf("년도 범위가 맞지 않습니다. 날짜를 다시 입력해주세요. \n");
						return 1;
					}
					if (nums[0] >= 2000 && nums[0] <= 2999) {
						
						if (nums[1] == 1 || nums[1] == 3 || nums[1] == 5 || nums[1] == 7
							|| nums[1] == 8 || nums[1] == 10 || nums[1] == 12) {
							if (z1 == 2) {
								printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
								return 1;
							}
							if (nums[2] >= 1 && nums[2] <= 31) {
								//printf("나중에 데이터베이스에 날짜 나눠서 넣기\n");
								if (z2 == 2) {
									printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
									return 1;
								}
								string y = to_string(nums[0]);
								string m = to_string(nums[1]);
								string d = to_string(nums[2]);
								*search_date = y + '-' + m + '-' + d;
								
								return 0;
							}
							else {
								printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
								return 1;
							}
						}
						else if (nums[1] == 4 || nums[1] == 6 || nums[1] == 9 || nums[1] == 11) {
							if (nums[2] >= 1 && nums[2] <= 30) {
								if (z1 == 2) {
									printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
									return 1;
								}
								//printf("나중에 데이터베이스에 날짜 나눠서 넣기2\n");
								if (z2 == 2) {
									printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
									return 1;
								}
								string y = to_string(nums[0]);
								string m = to_string(nums[1]);
								string d = to_string(nums[2]);
								*search_date = y + '-' + m + '-' + d;
								return 0;
							}
							else {
								printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
								return 1;
							}
						}
						else if (nums[1] == 2) {
							if (z1 == 2) {
								printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
								return 1;
							}
							int k;
							k = nums[0] % 4;
							if (nums[0] % 4 == 0 && nums[0] % 100 != 0 || nums[0] % 400 == 0) {
								if (nums[2] >= 1 && nums[2] <= 29) {
									//printf("나중에 데이터베이스에 날짜넣기3\n");
									if (z2 == 2) {
										printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
										return 1;
									}
									string y = to_string(nums[0]);
									string m = to_string(nums[1]);
									string d = to_string(nums[2]);
									*search_date = y + '-' + m + '-' + d;
									return 0;
								}
								else {
									printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
									return 1;
								}
							}
							else {
								if (nums[2] >= 1 && nums[2] <= 28) {
									if (z2 == 2) {
										printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
										return 1;
									}
									//printf("나중에 데이터베이스에 날짜넣기4\n");
									string y = to_string(nums[0]);
									string m = to_string(nums[1]);
									string d = to_string(nums[2]);
									*search_date = y + '-' + m + '-' + d;
									return 0;
								}
								else {
									printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
									return 1;
								}
							}
						}
						else {
							printf("월 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
							return 1;
						}
					}
					else {
						printf("년도 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
						return 1;
					}
				}
				else {
					printf("구분문자가 통일하지 않습니다. 날짜를 다시 입력해주세요.\n");
					return 1;
				}
			}//년도 까지 다입력한거
			else if (i == 2) {
				//년도를 입력하지 않았기 때문에 자동으로 컴퓨터 년도 설정해주기
				time_t base_time = time(NULL);
				struct tm* newtime = localtime(&base_time);
				int t = newtime->tm_year + 1900; //로컬년도설정 데이터베이스에 년도 자동으로 넣기 올바를때
				
				if (nums[0] == 1 || nums[0] == 3 || nums[0] == 5 || nums[0] == 7
					|| nums[0] == 8 || nums[0] == 10 || nums[0] == 12) {
					if (z3 == 2) {
						printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
						return 1;
					}
					if (nums[1] >= 1 && nums[1] <= 31) {
						if (z4 == 2) {
							printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
							return 1;
						}
						// printf("나중에 데이터베이스에 날짜 나눠서 넣기6\n"); // t랑 월이랑 일
						string y = to_string(t);
						string m = to_string(nums[0]);
						string d = to_string(nums[1]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
						return 1;
					}
				}
				else if (nums[0] == 4 || nums[0] == 6 || nums[0] == 9 || nums[0] == 11) {
					if (z3 == 2) {
						printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
						return 1;
					}
					if (nums[1] >= 1 && nums[1] <= 31) {

						//printf("나중에 데이터베이스 7\n");
						if (z4 == 2) {
							printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
							return 1;
						}
						string y = to_string(t);
						string m = to_string(nums[0]);
						string d = to_string(nums[1]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
						return 1;
					}
				}
				else if (nums[0] == 2) {
					if (z3 == 2) {
						printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
						return 1;
					}
					int k;
					k = t % 4;
					if (t % 4 == 0 && t % 100 != 0 || t % 400 == 0) {
						if (nums[1] >= 1 && nums[1] <= 29) {
							// printf("윤년 날짜넣기2 \n");
							if (z4 == 2) {
								printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
								return 1;
							}
							string y = to_string(t);
							string m = to_string(nums[0]);
							string d = to_string(nums[1]);
							*search_date = y + '-' + m + '-' + d;
							return 0;
						}
						else {
							printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
							return 1;
						}
					}
					else {
						if (nums[1] >= 1 && nums[1] <= 28) {
							//printf("평년 날짜 넣기2\n");
							if (z4 == 2) {
								printf("선행문자 '0'이 너무 많습니다. 날짜를 다시 입력해주세요. \n");
								return 1;
							}
							string y = to_string(t);
							string m = to_string(nums[0]);
							string d = to_string(nums[1]);
							*search_date = y + '-' + m + '-' + d;
							return 0;
						}
						else {
							printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
							return 1;
						}
					}
				}
				else {
					printf("월 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
					return 1;
				}
			}
			else {
				printf("형식에 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
				return 1;
			}//일이나 월만입력한거 , 구분문자 있을경우
		}// printf("구분문자잇음");
	}// 구분문자있는거 종료
	else {// printf("구분문자없음");
	   //먼저 년도입력, 년도 안입력으로 구분 4자리 2자리 2자리

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
						//printf("데베에 넣기\n");
						string y = to_string(nums[0]);
						string m = to_string(nums[1]);
						string d = to_string(nums[2]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
						return 1;
					}

				}
				else if (nums[1] == 4 || nums[1] == 6 || nums[1] == 9 || nums[1] == 11) {
					if (nums[2] >= 1 && nums[2] <= 30) {
						//printf("데베에 넣기\n");
						string y = to_string(nums[0]);
						string m = to_string(nums[1]);
						string d = to_string(nums[2]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
						return 1;
					}
				}
				else if (nums[1] == 2) {

					if (nums[0] % 4 == 0 && nums[0] % 100 != 0 || nums[0] % 400 == 0) {
						if (nums[2] >= 1 && nums[2] <= 29) {
							//printf("나중에 데이터베이스에 날짜넣기3\n");
							string y = to_string(nums[0]);
							string m = to_string(nums[1]);
							string d = to_string(nums[2]);
							*search_date = y + '-' + m + '-' + d;
							return 0;
						}
						else {
							printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
							return 1;
						}
					}
					else {
						if (nums[2] >= 1 && nums[2] <= 28) {
							//printf("나중에 데이터베이스에 날짜넣기4\n");
							string y = to_string(nums[0]);
							string m = to_string(nums[1]);
							string d = to_string(nums[2]);
							*search_date = y + '-' + m + '-' + d;
							return 0;
						}
						else {
							printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
							return 1;
						}
					}
				}
				else {
					printf("월 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
					return 1;
				}
			}
			else {
				printf("년도 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
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
					//printf("데베 넣기2\n");
					string y = to_string(t);
					string m = to_string(nums[0]);
					string d = to_string(nums[1]);
					*search_date = y + '-' + m + '-' + d;
					return 0;
				}
				else {
					printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
					return 1;
				}
			}
			else if (nums[0] == 4 || nums[0] == 6 || nums[0] == 9 || nums[0] == 11) {
				if (nums[1] >= 1 && nums[1] <= 30) {
					//printf("데베 넣기2\n");
					string y = to_string(t);
					string m = to_string(nums[0]);
					string d = to_string(nums[1]);
					*search_date = y + '-' + m + '-' + d;
					return 0;
				}
				else {
					printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
					return 1;
				}
			}
			else if (nums[0] == 2) {

				if (t % 4 == 0 && t % 100 != 0 || t % 400 == 0) {
					if (nums[1] >= 1 && nums[1] <= 29) {
						//printf("윤년 날짜넣기2 \n");
						string y = to_string(t);
						string m = to_string(nums[0]);
						string d = to_string(nums[1]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
						return 1;
					}
				}
				else {
					if (nums[1] >= 1 && nums[1] <= 28) {
						//printf("평년 날짜 넣기2\n");
						string y = to_string(t);
						string m = to_string(nums[0]);
						string d = to_string(nums[1]);
						*search_date = y + '-' + m + '-' + d;
						return 0;
					}
					else {
						printf("일 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
						return 1;
					}
				}
			}
			else {
				printf("월 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
				return 1;
			}
		}
		else {
			printf("문자열이 올바르지 않습니다. 날짜를 다시 입력해주세요. \n");
			return 1;
		}

	}

	return 0;
}


int onjum(char a[]) { //(김훈기) 구분문자 통일함수
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
int slash(char a[]) { //(김훈기) 구분문자 통일함수
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
int space(char a[]) { //(김훈기) 구분문자 통일함수
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
int minus1(char a[]) { //(김훈기) 구분문자 통일함수
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

int year1(char a[]) { // 년도있으면 다가능 6,7,8 (김훈기) 구분문자 없을시 년도 구분
	int j; int num = 0;
	for (j = 0; j < 4; j++) {
		num = num * 10 + a[j] - '0';
	}
	return num;
}

int month1(char a[]) {//8자리일 경우 (김훈기) 구분문자 없을시 월 구분
	int j; int num = 0;
	for (j = 4; j < 6; j++) {
		num = num * 10 + a[j] - '0';
	}
	return num;
}

int date1(char a[]) {//8자리일경우 (김훈기) 구분문자 없을시 일 구분
	int j; int num = 0;
	for (j = 6; j < 8; j++) {
		num = num * 10 + a[j] - '0';
	}
	return num;
}

int date2(char a[]) {//7자리 , (김훈기) 구분문자 없을시 일  구분
	int j; int num = 0;
	for (j = 2; j < 4; j++) {
		num = num * 10 + a[j] - '0';
	}
	return num;
}
int month2(char a[]) { //(김훈기) 구분문자 없을시 월 구분
	int j; int num = 0;
	for (j = 0; j < 2; j++) {
		num = num * 10 + a[j] - '0';
	}
	return num;
}



int sign(char a[]) { // (김훈기)잘못된 문자 확인
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

int signmiss(char a[]) { //(김훈기) 구분문자 연속으로 나오는지 확인
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

int zerogu4(char a[]) { //(김훈기) 년도에 선행문자 0
	int j; int num = 0;
	int len = strlen(a);
	if (a[0] == '0') {
		return 2;
	}

	return 1;
}

int zerogu1(char a[]) { //(김훈기) 선행문자 '0'이 2번 구분문자 o ,년도 o 월
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


int zerogu3(char a[]) { //(김훈기) 선행문자 '0'이 2번 구분문자 o ,년도 o 일
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

int zerogu5(char a[]) { //(김훈기) 선행문자 '0'이 2번 구분문자 o ,년도 x 월
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

int zerogu6(char a[]) { //(김훈기) 선행문자 '0'이 2번 구분문자 o ,년도 x 일
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

int last(char a[]) { //(김훈기) 선행문자 '0'이 2번 구분문자 o ,년도 x 일
	int j; int num = 0;
	int len = strlen(a)-1;
		if (a[len] == ' ' || a[len] == '-' || a[len] == '/' || a[len] == '.') {
				return 2;
	}
	return 1;
}


int isnum(char letter) {
	//문자가 숫자이면 1반환 아니면 0 반환하는 함수

	int x = 1;
	char* stop; //문자열에 나오는 첫 숫자가 아닌 문자 위치의 포인터 저장
	strtol(&letter, &stop, 10);

	//만약 문자면 0 반환
	if (letter == *stop)x = 0;
	return x;
}

//숫자형태의 시간에 대해 의미오류를 확인해주는 함수
int time_meaning_check(char* starting_time, int type) {
	int time;//최종 형태 시간값
	int hour = 00, minute;
	if (type == 1) {
		//1.hhmm
		if (strlen(starting_time) == 4 && isnum(starting_time[1]) && isnum(starting_time[2]) && isnum(starting_time[3])) {
			hour += atoi(starting_time) / 100;
			minute = atoi(starting_time) % 100;
			//printf("hhmm\n");
			//printf("%d시 %d분\n", hour, minute);
			time = hour * 100 + minute;
			if (hour > 24 || hour < 0 || minute < 0 || minute >= 60 || (hour == 24 && minute != 0)) {
				printf("존재하지 않는 시간입니다. 다시 입력해주세요.\n");
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
			//printf("%d시 %d분\n", hour, minute);
			time = hour * 100 + minute;
			if (hour > 24 || hour < 0 || minute < 0 || minute >= 60 || (hour == 24 && minute != 0)) {
				printf("존재하지 않는 시간입니다. 다시 입력해주세요.\n");
				return -1;
			}

		}
		//3. 정해진 형식이 아닐때
		else {
			printf("올바른 시간형식이 아닙니다. \"hh:mm\", \"hhmm\", \"hh mm\" 중 하나의 형식으로 입력해주세요.\n");
			return -1;
		}
	}
	else if (type == 2 || type == 3) { //type2 -> 일때 00:00~12:00까지 입력받음
		//1.hhmm
		if (strlen(starting_time) == 4 && isnum(starting_time[1]) && isnum(starting_time[2]) && isnum(starting_time[3])) {
			if (type == 3) hour += atoi(starting_time) / 100 + 12;	//type3인경우 12 더하기
			else hour += atoi(starting_time) / 100;
			minute = atoi(starting_time) % 100;
			//printf("hhmm\n");
			//printf("%d시 %d분\n", hour, minute);
			time = hour * 100 + minute;
			if (type == 2 && (hour > 12 || hour < 0 || minute < 0 || minute >= 60 || (hour == 12 && minute != 0))) {
				printf("존재하지 않는 시간입니다. 다시 입력해주세요.\n");
				return -1;
			}
			if (type == 3 && (hour > 24 || hour < 0 || minute < 0 || minute >= 60 || (hour == 24 && minute != 0))) {
				printf("존재하지 않는 시간입니다. 다시 입력해주세요.\n");
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

			if (type == 3) hour += atoi(hh) + 12;	//type3인경우 12 더하기
			else hour += atoi(hh);
			minute = atoi(mm);
			//printf("hh:mm hh mmm\n");
			//printf("%d시 %d분\n", hour, minute);
			time = hour * 100 + minute;
			if (type == 2 && (hour > 12 || hour < 0 || minute < 0 || minute >= 60 || (hour == 12 && minute != 0))) {
				printf("존재하지 않는 시간입니다. 다시 입력해주세요.\n");
				return -1;
			}
			if (type == 3 && (hour > 24 || hour < 0 || minute < 0 || minute >= 60 || (hour == 24 && minute != 0))) {
				printf("존재하지 않는 시간입니다. 다시 입력해주세요.\n");
				return -1;
			}


		}

		//3. 정해진 형식이 아닐때
		else {
			printf("올바른 시간형식이 아닙니다. \"hh:mm\", \"hhmm\", \"hh mm\" 중 하나의 형식으로 입력해주세요.\n");
			return -1;
		}
	}

	return time;

}





void time_grammar_check(int start_or_end, string& time_in_string) {

	char time_input[50];		// 입력받은 시간 문자열
	char first_letter;
	int hour, minute;
	int start_time = 0;		// DB형식에 맞게 가공된 시간값
	int type; //1:숫자로만 이루어진 문자열, 2:오전,am,AM이 포함된 시간문자열, 3:오후,pm,PM이 포함된 시간문자열
	int time = -1; //DB시간 형태에 맞게 가공된 시간 값 이것을 리턴해야함

	rewind(stdin); // 잉여 키값 초기화
	while (1) {
		if (start_or_end == 0) {
			printf("시작시간을 입력해주세요: ");

			//#### 프로세스 강제 종료시 (강민호)
			try {
				if (gets_s(time_input) == 0)
					throw start_time;
			}
			catch (int err) {
				printf("모든 내용을 입력하지 않아 저장되지 않았습니다.\n");
				Sleep(3000);
				exit(1);
			}
		}
		else {
			printf("마감시간을 입력해주세요: ");
			//#### 프로세스 강제 종료시 (강민호)
			try {
				if (gets_s(time_input) == 0)
					throw start_time;
			}
			catch (int err) {
				printf("모든 내용을 입력하지 않아 저장되지 않았습니다.\n");
				Sleep(3000);
				exit(1);
			}
		}


		//숫자로만 이루어진 시간 표현: 첫문자가 글자인지 숫자인지 여부 판단
		if (isnum(time_input[0])) {
			//숫자 일때
			//printf("숫자로만 이루어진 시간 표현\n");
			type = 1; //문자열 type 구분
			time = time_meaning_check(time_input, type);


		}

		else {//문자가 포함된 시간 표현:
			//printf("문자 포함된 시간표현");
			if (strstr(time_input, "오전 ") == time_input || strstr(time_input, "오후 ") == time_input) {
				//문자열 type 구분
				if (strstr(time_input, "오후 ") == time_input) type = 3;
				else type = 2;

				//문자를 제외한 시간 부분 추출
				for (int i = 5; i < 50; i++) {
					time_input[i - 5] = time_input[i];
				}
				//의미오류 확인
				if (isnum(time_input[0])) {
					time = time_meaning_check(time_input, type);

				}
				//문법오류: 숫자가 아닌 문자열인 경우
				else {
					printf("숫자 형태로 시간을 입력해주세요! \"hh:mm\", \"hhmm\", \"hh mm\" 중 하나의 형식만 가능합니다.\n");
				}
			}
			else if (strstr(time_input, "am ") == time_input || strstr(time_input, "AM ") || strstr(time_input, "pm ") == time_input || strstr(time_input, "PM ") == time_input) {
				//문자열 type 구분 
				if (strstr(time_input, "pm ") == time_input || strstr(time_input, "PM ") == time_input) type = 3;
				else type = 2;

				//문자를 제외한 시간 부분 추출
				for (int i = 3; i < 50; i++) {
					time_input[i - 3] = time_input[i];
				}
				//의미오류 확인
				if (isnum(time_input[0])) {
					time = time_meaning_check(time_input, type);

				}
				//문법오류: 숫자가 아닌 문자열인 경우
				else {
					printf("숫자 형태로 시간을 입력해주세요! \"hh:mm\", \"hhmm\", \"hh mm\" 중 하나의 형식만 가능합니다.\n");
				}
			}
			else {
				printf("사용 불가능한 문자열입니다! 문자는 \"오전\", \"am\",\"AM\",\"오후\",\"pm\",\"PM\" 중 하나만 사용가능 합니다. 해당 문자열 입력이후 한 칸을 띄고 시간을 입력해주세요.\n");
			}
		}
		if (time != -1) break;	//time값이 정상 리턴된 경우 반복문 탈출
	}
	//printf("%d", time);


	if (time == 0) 	time_in_string = "0000";
	else if (time < 100) time_in_string = "00" + to_string(time);
	else if (time < 1000)		time_in_string = "0" + to_string(time);
	else time_in_string = to_string(time);

}

int time_interval_check1(vector<string>* db, string time_in_string, int start_or_end) {//입력된 시간이 저장된 시간구간 안에 포함되는지 여부 확인

	int time = stoi(time_in_string);//시간 문자열을 int형으로 바꾸어줌 시, 분을 나누지 않고 하나의 정수로 크기비교 가능
	stringstream stream_test;

	int num, year, month, day, start_time, end_time;

	int length = (*db).size();
	for (int i = 0; i < length; i++)
	{
		stream_test.clear(); // 다시 초기화
		stream_test.str((*db).at(i)); // 이번엔 해당 일자의 DB만 가져옴.

		stream_test >> num >> year >> month >> day >> start_time >> end_time;
		if (start_or_end == 1) {	//마감시간인경우
			if (time > start_time && time <= end_time) {// 저장된 시간구간안에 존재하는 시간입력
				printf("해당 시각에 이미 입력된 활동이 있습니다. \n");
				return 1;
			}
		}

		if (start_or_end == 0) {	//시작시간인 경우
			if (time >= start_time && time < end_time) {// 저장된 시간구간안에 존재하는 시간입력
				printf("해당 시각에 이미 입력된 활동이 있습니다. \n");
				return 1;
			}
		}
	}
	return 0;

}


int time_interval_check2(vector<string>* db, string start_time_in_string, string end_time_in_string) { //입력된 시간구간안에 앞서 저장되어 있는 활동이 포함되는지 여부 확인
	int initial_time = stoi(start_time_in_string);//입력된 시작시간시간 문자열을 int형으로 바꾸어줌 시, 분을 나누지 않고 하나의 정수로 크기비교 가능
	int final_time = stoi(end_time_in_string);	//입력된 마감시간

	stringstream stream_test;

	int num, year, month, day, start_time, end_time;

	int length = (*db).size();

	for (int i = 0; i < length; i++) {
		stream_test.clear(); // 다시 초기화
		stream_test.str((*db).at(i)); // 이번엔 해당 일자의 DB만 가져옴.

		stream_test >> num >> year >> month >> day >> start_time >> end_time;
		if ((initial_time <= start_time && final_time > start_time) || (initial_time < end_time && final_time >= end_time)) {// 저장된 시간구간안에 존재하는 시간입력
			printf("입력된 시간구간 사이에 이미 입력된 활동이 있습니다. \n");
			return 1;
		}
	}
	return 0;
}


void DailyChart::Schedule_make(vector<string>* db, string date_data) {

	/* 인자 설명

	vector<string> db : 해당 일자의 일정 정보만 저장된 DB 포인터 인자.

	string date_data : 저장할 일과 날짜 정보. 한줄 받기로 string 형태로 받았음.

	*/
	rewind(stdin); // 잉여 키값 초기화

	vector<string> DB_test;

	All_Database.clear();
	Daily_DB_Read(&All_Database); // 현재 전체 DB 추출

	int D_year = 0, D_month = 0, D_day = 0;
	int last_num; // db에 있는 마지막 정보의 Id_num(고유번호)를 받을 변수. last_num + 1 로 고유번호를 저장하기 위해서다.
	int i = 0;
	//int s = 0;//카테고리 오류 검사 변수
	char a = 'a';
	char test[10000];
	int test_option= 0;

	stringstream stream_test;
	stream_test.str(date_data);


	stream_test >> D_year >> D_month >> D_day; // 년월일 구분
	stream_test.clear();

	/*stringstream stream_test2;*/
	if (All_Database.back().empty() == 0) // 마지막행이 엔터 안되어있으면
	{
		test_option = 1;
	}
	else // 엔터되어있으면 엔터된 빈값만 뺌.
	{
		All_Database.pop_back();
	}
	
	stream_test.str((All_Database).back()); // 전체 DB의 (마지막-1)행 뽑음. => 마지막 P_ID를 추출하기 위함. 마지막행은 빈칸임.

	stream_test >> num; // 최종 P_ID 추출


	last_num = num;

	D_year = abs(D_year);
	D_month = abs(D_month);
	D_day = abs(D_day);


	num = last_num + 1; // 새로 입력할 일정은 마지막 P_ID의 +1된 숫자


	//#### 여기서 겹치는 시간대가 있는지 체크해야함.

	//시간 입력(이정수)
	while (1) {
		while (1) {

			time_grammar_check(0, start_time);	// 문법오류, 의미오류 검사 후 시간 표현 표준화시킴
			if (time_interval_check1(db, start_time, 0) == 0) break;	//저장된 시간구간 안에 포함되는지 여부 확인
		}
		while (1) {
			time_grammar_check(1, end_time);	// 문법오류, 의미오류 검사 후 시간 표현 표준화시킴
			if (stoi(end_time) <= stoi(start_time)) {//마감시간이 시작시간 뒤에 오는지 확인
				printf("마감시간은 시작시간보다 뒤에오는 시간이어야합니다.\n");
				continue;
			}
			if (time_interval_check1(db, end_time, 1) == 0) break;	//저장된 시간구간 안에 포함되는지 여부 확인
		}
		if (time_interval_check2(db, start_time, end_time) == 0) break;
	}


	// 시간 차이 계산

	/*
	이 함수에서 시간은 '0600', "06:00"(6시 정각) 같은 형태로 받아들였다는 것을 전제로 한다. 그 이외는 생각하지 않는다.
	그래서 예외처리때 위의 형태로 표준화를 시킴.
	*/
	stream_test.clear();


	int start_minute, end_minute, start_hour, end_hour;
	start_hour = atoi(start_time.substr(0, 2).c_str()); // 몇 시에 시작했는지를 string에서 int로 전환
	end_hour = atoi(end_time.substr(0, 2).c_str()); // 종료 시를 string에서 int로 전환
	start_minute = atoi(start_time.substr(start_time.length() - 2, 2).c_str()); // 시작 분 string에서 int로 전환
	end_minute = atoi(end_time.substr(start_time.length() - 2, 2).c_str()); // 종료 분 string에서 int로 전환


	/*
	함수 설명

	atoi(char *) : char *형 배열을 int로 전환해줌

	c_str() : C++의 string을  C언어의 char* 형 배열로 바꿔줌

	*/


	btw_time = ReturnBtwTime(start_minute, end_minute, start_hour, end_hour);



	//카테고리(이일진 & 강민호)


	string t;
	stringstream s1;
	int p;

	while (1) {
		rewind(stdin); // 잉여 키값 초기화
		test[0] = '\0'; // 버퍼초기화

		printf("활동을 선택해 주세요(1.수면, 2.식사, 3.수업, 4.공부, 5.운동, 6.취미, 7.아르바이트, 8.기타): ");

		//#### 강제 종료 예외처리(강민호)
		try {
			if (gets_s(test) == 0)
			{
				throw end_time;
			}
		}
		catch (string err2) {
			printf("모든 내용을 입력하지 않아 저장되지 않았습니다.\n");
			Sleep(3000);
			exit(1);
		}

		cartegory = test;


		if (cartegory == "1") {
			cartegory = "수면";
			break;
		}
		else if (cartegory == "2") {
			cartegory = "식사";
			break;
		}
		else if (cartegory == "3") {
			cartegory = "수업";
			break;
		}
		else if (cartegory == "4") {
			cartegory = "공부";
			break;
		}
		else if (cartegory == "5") {
			cartegory = "운동";
			break;
		}
		else if (cartegory == "6") {
			cartegory = "취미";
			break;
		}
		else if (cartegory == "7") {
			cartegory = "아르바이트";
			break;
		}
		else if (cartegory == "8") {
			cartegory = "기타";
			break;
		}
		else if (atoi(cartegory.c_str()) == 0 && cartegory != "0") { // atoi는 해석되는 int값을 리턴하고, 해석을 못했을 때 0을 리턴한다.
			// 즉 0으로 리턴했다는 것은 문자열을 입력 받았다는 소리다.
			printf("'%s'는 문자열 입니다! 1-8사이의 숫자를 입력해주세요.:\n", cartegory.c_str());

		}
		else { // 나머지는 선두 숫자를 받은 문자열이거나(ex. 1이지롱), 범위에 맞지 않는 숫자들이다.
			

			s1.clear();
			t.clear();

			s1.str(cartegory);

			if (detail_chart.size() > 1) // 1글자 이상이면 -> 문자열일 확률 높음
			{

				s1 >> p >> t;

				if (t.empty()) // string이 비어있으면 카테고리는 숫자열
				{
					printf("잘못된 입력입니다. 1부터 8사이의 숫자를 입력해주세요.\n");
				}
				else // 아니면 문자열
				{
					printf("'%s'는 문자열 입니다! 1-8사이의 숫자를 입력해주세요.:\n", cartegory.c_str());
				}
			}
			else
			{
				printf("잘못된 입력입니다. 1부터 8사이의 숫자를 입력해주세요.\n");
			}
		
			/*
			for (i = 0; i < cartegory.size(); ++i) {

				if (!isdigit(cartegory[i])) {//isdigit()매개변수가 숫자면 0이 아닌 값 매개변수가 숫자가 아니면 0을 반환

					++s;

				}

			}
			if (s > 0) {
				printf("'%s'는 문자열 입니다! 1-8사이의 숫자를 입력해주세요.:\n", cartegory.c_str());
				s = 0;
			}
			else {

				printf("잘못된 입력입니다. 1부터 8사이의 숫자를 입력해주세요.\n");
			}*/

		}

	}

	rewind(stdin); // 잉여 키값 초기화 


	//세부내용

	while (1) {
		cout << "세부내용을 쓰세요 : ";
		test[0] = '\0'; // 버퍼 초기화

		//#### 프로세스 강제 종료시 (강민호)
		try {
			//std::getline(cin, detail_chart);
			if (gets_s(test) == 0) {
				throw a;
			}

			cout << "" << endl;
		}
		catch (char err) {
			printf("모든 내용을 입력하지 않아 저장되지 않았습니다.\n");
			Sleep(3000);
			exit(1);
		}

		detail_chart = test;

		//세부내용 오류검사

		if (CheckDetail(&detail_chart)) { // 오류검사 통과하면 1이 리턴되서 break됨.

			break;
		}
	}


	string str;

	// 받은 정보로 하나의 string 만들기

	if (test_option) // 마지막행에 엔터가 안쳐져 있었으면
	{
		str = '\n' + to_string(num) + '\t' + to_string(D_year) + '\t' + to_string(D_month) + '\t' + to_string(D_day) + '\t' +
			start_time + '\t' + end_time + '\t' + to_string(btw_time) + '\t' + cartegory + '\t' + detail_chart + '\n';
	}
	else //쳐저 있으면
	{
		str = to_string(num) + '\t' + to_string(D_year) + '\t' + to_string(D_month) + '\t' + to_string(D_day) + '\t' +
			start_time + '\t' + end_time + '\t' + to_string(btw_time) + '\t' + cartegory + '\t' + detail_chart + '\n';
	}

	
	// 파일 이어 쓰기

	ofstream WriteFile("DB.txt", ios::app);

	WriteFile << str;

	printf("저장되었습니다!\n");

	WriteFile.close();

	rewind(stdin); // 잉여 키값 초기화 
	while (!kbhit()) // 아무키 입력 받으면 종료되는 것
	{

	}

}


int DailyChart::CheckP_ID(vector<string>* db, string id) {// P_ID 오류 체크 함수 ####
	/*오류에 따라 리턴값을 달리 할 것*/

	stringstream stream_test;

	int ex_check = 0; // 해당 일자에 해당 P_ID가 존재하는 지 여부 체크 변수
	int i, s = 0;// 고유번호 오류 검사 변수


	/*####우선 P_ID가 오류가 없는지 확인*/

	if (atoi(id.c_str()) == 0 && id != "0") {// 오류가 있으면 (문자열을 입력 받았을 때)
		printf("존재하지 않는 고유번호 입니다.\n");
		return 0;
	}
	else {
		for (i = 0; i < id.size(); ++i) {

			if (!isdigit(id[i])) {//isdigit()매개변수가 숫자면 0이 아닌 값 매개변수가 숫자가 아니면 0을 반환

				++s; // 문자가 있으면 카운트가 올라감

			}

		}

		if (s > 0) { // 이건 '1이지롱' 같은 선두에 숫자가 포함된 문자열
			printf("존재하지 않는 고유번호 입니다.\n");
			s = 0;
			return 0;
		}
	}

	//이걸로 숫자만 온전히 걸러짐


	// 이제 P_ID가 해당 일자에서 존재하는 번호인지 확인.

	for (unsigned int i = 0; i < (*db).size(); i++) {// db 크기 만큼 모든 행을 조사함.
		/*
		현재 DB는 db[0] = "1\t2020\t03\t20\t0600\t0900\t180\t식사\t아침식사"
		이런 식으로 저장이 되어있다.

		테이블 형식이 순서대로 int, int, int, int, string, string, int, string, string 이므로

		이 순서대로 sstream을 이용해 인자를 받는다.

		여기서 vector은 string 형이기에 db[i]의 모든 내용은 다 string 형인 점을 기억하자.
		*/


		stream_test.clear(); // sstream은 한번 쓰고 나면 다시 초기화 시켜야 쓸 수 있다.

		stream_test.str((*db)[i]);

		stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
			cartegory >> detail_chart; // 클래스 인자 구분


		if (atoi(id.c_str()) == num) {// 고유번호가 일치하면
			ex_check = 1;
			// 해당 번호는 하나만 존재하므로 for문을 끝냄.
			break; // 고유번호는 하나 밖에 없기 때문에 break해야 메모리 아낌.

		}
		else { // 일치 안하는 정보면
			//일단 넘어감.

		}


	}

	if (ex_check) {// 존재하는 번호면
		return 1;
	}
	else {// 존재하지 않는 번호면
		//존재하지 않는다고 출력
		printf("존재하지 않는 고유번호입니다.\n");
		return 0;
	}

}

void DailyChart::Schedule_delete(vector<string>* db, string date_data)// 일정 삭제 함수
{
	/* 삭제 원리 설명

	원하는 P_ID를 입력받고 전체 DB에서 해당 행을 찾아 삭제한 후,
	남은 DB로 깡그리 덮어쓰는 방식이다.

	*/

	rewind(stdin); // 잉여 키값 초기화

	vector<string> DB_test; // 여기에 삭제할 정보 빼고 다시 다 저장할 것임.

	All_Database.clear();

	Daily_DB_Read(&All_Database); // 현재 전체 DB 추출

	int D_year = 0, D_month = 0, D_day = 0;
	string p_id;
	int index_count = 0;

	stringstream stream_test;
	stream_test.str(date_data);

	char test[10000];

	stream_test >> D_year >> D_month >> D_day; // 년월일 구분
	stream_test.clear();

	D_year = abs(D_year);
	D_month = abs(D_month);
	D_day = abs(D_day);

	if ((*db).size() != 0) {
		while (1) {
			printf("삭제하실 활동의 고유번호를 선택해주세요: ");

			//#### 프로세스 강제 종료시 (강민호)
			try {
				//std::getline(cin, p_id);

				if (gets_s(test) == 0) {
					throw test;
				}

				cout << "" << endl;
			}
			catch (char *err) {
				printf("모든 내용을 입력하지 않아 저장되지 않았습니다.\n");
				Sleep(3000);
				exit(1);
			}

			//p_id 오류 검사

			p_id = test;

			if (CheckP_ID(db, p_id)) { // 오류가 없으면
				break;
			}
			else {// 오류가 있으면
				//오류 출력
			}
		}

		(*db).clear();

		//p_id와 맞는 행 번호를 찾기 위해 모든 DB를 조사함.

		for (unsigned int i = 0; i < (All_Database).size() - 1; i++) {
			/*
			현재 DB는 db[0] = "1\t2020\t03\t20\t0600\t0900\t180\t식사\t아침식사"
			이런 식으로 저장이 되어있다.

			테이블 형식이 순서대로 int, int, int, int, string, string, int, string, string 이므로

			이 순서대로 sstream을 이용해 인자를 받는다.

			여기서 vector은 string 형이기에 db[i]의 모든 내용은 다 string 형인 점을 기억하자.
			*/


			stream_test.clear(); // sstream은 한번 쓰고 나면 다시 초기화 시켜야 쓸 수 있다.

			stream_test.str((All_Database)[i]);

			stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
				cartegory >> detail_chart; // 클래스 인자 구분


			if (atoi(p_id.c_str()) == num) {// 고유번호가 일치하면
				// 이것만 저장하지 않음.

			}
			else { // 일치 안하는 정보면
				if (D_year == year && D_month == month && D_day == day)
					(*db).push_back(All_Database[i]);

				(DB_test).push_back(All_Database[i]); // 읽어온 정보를 DB_test에 차곡차곡 저장
			}


		}



		// 삭제를 마친 DB를 만들었으니 이제 이를 덮어쓰면 된다.

		ofstream WriteFile_1("DB.txt", ios::out); // out은 기존 내용을 전부 삭제하고 처음부터 쓰겠단 의미
		string str = DB_test[0] + '\n';

		WriteFile_1 << str;
		WriteFile_1.close();


		for (unsigned int i = 1; i < (DB_test).size(); i++) {
			ofstream WriteFile_2("DB.txt", ios::app); //app은 기존 내용에 이어쓰겠단 의미.
			str = DB_test[i] + '\n';
			WriteFile_2 << str;
			WriteFile_2.close();
		}


		printf("저장되었습니다!\n");

		rewind(stdin); // 잉여 키값 초기화 
		while (!kbhit()) // 아무키 입력 받으면 종료되는 것
		{

		}
	}
}


void DailyChart::Schedule_modify(vector<string>* db, string date_data) {// 일정 수정 함수
	/* 수정 원리 설명

	일치하는 날짜를 찾아 해당 DB인자를 바꾸고
	이를 다시 덮어쓰는 방식이다.
	*/

	/* 인자 설명

	vector<string> db : 해당 일자의 일정 정보만 저장된 DB 포인터 인자.

	string date_data : 저장할 일과 날짜 정보. 한줄 받기로 string 형태로 받았음.

	*/


	rewind(stdin); // 잉여 키값 초기화

	All_Database.clear();
	Daily_DB_Read(&All_Database); // 현재 전체 DB 추출

	vector<string> DB_test; // 해당 일자의 DB에서 수정할 행 뺀 나머지 DB

	int D_year = 0, D_month = 0, D_day = 0;
	string p_id; // 고유번호 입력 변수
	int id_num; // 고유번호와 맞는 정보가 들어있을 DB 행 번호 변수
	int i = 0;
	// int s = 0;//카테고리 오류 검사 변수

	char test[10000];
	char a = 'a';
	float b = 0.0;

	stringstream stream_test;
	stream_test.str(date_data);

	stream_test >> D_year >> D_month >> D_day; // 년월일 구분
	stream_test.clear();

	D_year = abs(D_year);
	D_month = abs(D_month);
	D_day = abs(D_day);

	if ((*db).size() != 0) {
		while (1) {

			printf("수정하실 활동의 고유번호를 선택해주세요: ");

			//#### 프로세스 강제 종료시 (강민호)
			try {
				//std::getline(cin, p_id);

				if (gets_s(test) == 0) {
					throw test;
				}

				cout << "" << endl;
			}
			catch (char *err) {
				printf("모든 내용을 입력하지 않아 저장되지 않았습니다.\n");
				Sleep(3000);
				exit(1);
			}

			p_id = test;

			//#### p_id 오류 검사


			if (CheckP_ID(db, p_id)) {// 오류가 없으면
				break;
			}

		}

		//p_id와 맞는 행 번호를 찾기 위해 모든 DB를 조사함.

		for (unsigned int i = 0; i < (All_Database).size() - 1; i++) {
			/*
			현재 DB는 db[0] = "1\t2020\t03\t20\t0600\t0900\t180\t식사\t아침식사"
			이런 식으로 저장이 되어있다.

			테이블 형식이 순서대로 int, int, int, int, string, string, int, string, string 이므로

			이 순서대로 sstream을 이용해 인자를 받는다.

			여기서 vector은 string 형이기에 db[i]의 모든 내용은 다 string 형인 점을 기억하자.
			*/


			stream_test.clear(); // sstream은 한번 쓰고 나면 다시 초기화 시켜야 쓸 수 있다.

			stream_test.str((All_Database)[i]);

			stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
				cartegory >> detail_chart; // 클래스 인자 구분

			if (D_day == day && D_month == month && D_year == year) {
				if (atoi(p_id.c_str()) == num) {// 고유번호가 일치하면
					//DB의 해당 행번호를 저장해둠.
					id_num = i;

				}
				else {
					DB_test.push_back(All_Database[i]); // 수정을 위해 따로 DB 저장
				}
			}

		}




		/* 이후는 Scheduel_make 함수를 참고하여 작성.*/


		//시간 입력(이정수)
		while (1) {

			while (1) {

				time_grammar_check(0, start_time);	// 문법오류, 의미오류 검사 후 시간 표현 표준화시킴
				if (time_interval_check1(&DB_test, start_time, 0) == 0) break;	//저장된 시간구간 안에 포함되는지 여부 확인
			}
			while (1) {
				time_grammar_check(1, end_time);	// 문법오류, 의미오류 검사 후 시간 표현 표준화시킴
				if (stoi(end_time) <= stoi(start_time)) {//마감시간이 시작시간 뒤에 오는지 확인
					printf("마감시간은 시작시간보다 뒤에 오는 시간이어야합니다.\n");
					continue;
				}
				if (time_interval_check1(&DB_test, end_time, 1) == 0) break;	//저장된 시간구간 안에 포함되는지 여부 확인
			}
			if (time_interval_check2(&DB_test, start_time, end_time) == 0) break;
		}



		stream_test.clear();


		int start_minute, end_minute, start_hour, end_hour;
		start_hour = atoi(start_time.substr(0, 2).c_str()); // 몇 시에 시작했는지를 string에서 int로 전환
		end_hour = atoi(end_time.substr(0, 2).c_str()); // 종료 시를 string에서 int로 전환
		start_minute = atoi(start_time.substr(start_time.length() - 2, 2).c_str()); // 시작 분 string에서 int로 전환
		end_minute = atoi(end_time.substr(start_time.length() - 2, 2).c_str()); // 종료 분 string에서 int로 전환


		/*
		함수 설명

		atoi(char *) : char *형 배열을 int로 전환해줌

		c_str() : C++의 string을  C언어의 char* 형 배열로 바꿔줌

		*/

		btw_time = ReturnBtwTime(start_minute, end_minute, start_hour, end_hour);




		//카테고리(이일진 & 강민호)

		string t;
		stringstream s1;
		int p;


		while (1) {
			rewind(stdin); // 잉여 키값 초기화
			test[0] = '\0'; // 버퍼 초기화
			cout << "활동을 선택해 주세요(1.수면, 2.식사, 3.수업, 4.공부, 5.운동, 6.취미, 7.아르바이트, 8.기타): ";

			//#### 강제 종료 예외처리
			try {
				//std::getline(cin, cartegory);
				if (gets_s(test) == 0) {
					throw start_time;
				}
			}
			catch (string err) {
				printf("모든 내용을 입력하지 않아 저장되지 않았습니다.\n");
				Sleep(3000);
				exit(1);
			}

			cartegory = test;

			if (cartegory == "1") {
				cartegory = "수면";
				break;
			}
			else if (cartegory == "2") {
				cartegory = "식사";
				break;
			}
			else if (cartegory == "3") {
				cartegory = "수업";
				break;
			}
			else if (cartegory == "4") {
				cartegory = "공부";
				break;
			}
			else if (cartegory == "5") {
				cartegory = "운동";
				break;
			}
			else if (cartegory == "6") {
				cartegory = "취미";
				break;
			}
			else if (cartegory == "7") {
				cartegory = "아르바이트";
				break;
			}
			else if (cartegory == "8") {
				cartegory = "기타";
				break;
			}
			else if (atoi(cartegory.c_str()) == 0 && cartegory != "0") {// atoi는 해석되는 int값을 리턴하고, 해석을 못했을 때 0을 리턴한다.  즉 0으로 리턴했다는 것은 문자열을 입력 받았다는 소리다.
				printf("'%s'는 문자열 입니다! 1-8사이의 숫자를 입력해주세요.:\n", cartegory.c_str());

			}
			else {// 나머지는 선두 숫자를 받은 문자열이거나(ex. 1이지롱), 범위에 맞지 않는 숫자들이다.
				
				s1.clear();
				t.clear();

				s1.str(cartegory);

				if (detail_chart.size() > 1) // 1글자 이상이면 -> 문자열일 확률 높음
				{

					s1 >> p >> t;

					if (t.empty()) // string이 비어있으면 카테고리는 숫자열
					{
						printf("잘못된 입력입니다. 1부터 8사이의 숫자를 입력해주세요.\n");
					}
					else // 아니면 문자열
					{
						printf("'%s'는 문자열 입니다! 1-8사이의 숫자를 입력해주세요.:\n", cartegory.c_str());
					}
				}
				else
				{
					printf("잘못된 입력입니다. 1부터 8사이의 숫자를 입력해주세요.\n");
				}
				/*
				for (i = 0; i < cartegory.size(); ++i) {

					if (!isdigit(cartegory[i])) {//isdigit()매개변수가 숫자면 0이 아닌 값 매개변수가 숫자가 아니면 0을 반환

						++s;

					}

				}
				if (s > 0) {
					printf("'%s'는 문자열 입니다! 1-8사이의 숫자를 입력해주세요.:\n", cartegory.c_str());
					s = 0;
				}
				else {

					printf("잘못된 입력입니다. 1부터 8사이의 숫자를 입력해주세요.\n");
				}*/

			}
		}





		//세부내용

		while (1) {
			rewind(stdin); // 잉여 키값 초기화 
			test[0] = '\0';
			printf("세부내용을 입력해주세요 : ");
			//cin >> detail_chart; 
			//cout << "" << endl;

			//#### 프로세스 강제 종료시 (강민호) 
			try {

				if (gets_s(test) == 0) {
					throw a;
				}

				cout << "" << endl;
			}
			catch (char err) {
				printf("모든 내용을 입력하지 않아 저장되지 않았습니다.\n");
				Sleep(3000);
				exit(1);
			}

			detail_chart = test;

			//#### 세부내용 오류검사해야함

			if (CheckDetail(&detail_chart)) { // 오류검사 통과하면 1이 리턴되서 break됨.
				break;
			}

		}



		// 받은 정보를 수정 DB에 저장하기.

		All_Database[id_num] = p_id + '\t' + to_string(year) + '\t' + to_string(month) + '\t' + to_string(day) + '\t' +
			start_time + '\t' + end_time + '\t' + to_string(btw_time) + '\t' + cartegory + '\t' + detail_chart;


		// 수정을 마친 DB를 만들었으니 이제 이를 덮어쓰면 된다.

		ofstream WriteFile_1("DB.txt", ios::out); // out은 기존 내용을 전부 삭제하고 처음부터 쓰겠단 의미
		string str = All_Database[0] + '\n';

		WriteFile_1 << str;
		WriteFile_1.close();


		for (unsigned int i = 1; i < (All_Database).size() - 1; i++) {
			ofstream WriteFile_2("DB.txt", ios::app); //app은 기존 내용에 이어쓰겠단 의미.
			str = All_Database[i] + '\n';
			WriteFile_2 << str;
			WriteFile_2.close();
		}


		printf("저장되었습니다!\n");



		rewind(stdin); // 잉여 키값 초기화 
		while (!kbhit()) // 아무키 입력 받으면 종료되는 것
		{

		}
	}
}







void menu_1() {
	rewind(stdin); // 잉여 키값 초기화
	system("cls");
	printf("*하루 일과 입력 및 수정*\n\n");

	int wrong_1 = 0; //오류 확인 변수
	string write_date; // 입력할날짜
	string write_dateo;
	DailyChart D_1; // 클래스 선언

	string menu_number_1; // 메뉴 변수


	vector<string> Database_1; // 메뉴 1에서만 사용할 데이터 베이스

	do {
		printf("날짜를 입력해주세요: ");

		std::getline(cin, write_date);
		write_dateo = write_date;
		printf("\n");
		wrong_1 = DateCheck(&write_date); // 날짜 오류 검사. 정상이면 0 리턴되서 다음 메뉴 출력. 오류가 있으면 1으로 리턴되서 다시 while문 수행
	} while (wrong_1);

	D_1.Daily_DB_Search(&Database_1, write_date, 1);  // 이제 write_date의 날짜에 해당되는 일과를 출력함.

	while (true) {
		if (wrong_1) // 기능을 수행하고 다시 돌아왔으면  

		{
			system("cls");
			printf("*하루 일과 입력 및 수정*\n\n");

			printf("날짜를 입력해주세요: %s\n\n", write_dateo.c_str());
			D_1.Daily_DB_Search(&Database_1, write_date, 1);  // 이제 write_date의 날짜에 해당되는 일과를 출력함.


		}
		else {
			
		}	

		wrong_1 = 0; // 다시 초기화

		printf("\n수행하실 기능을 선택해주세요(1.새로운 활동입력 2.기존 활동 삭제 3. 기존 활동 내용 수정 4.메뉴화면으로 돌아가기):");

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
			printf("\n잘못된 입력입니다. 1에서4 사이의 자연수를 입력해주세요\n");
		}


	}


}


int ReturnHour(int time) { // 시 반환 함수, 매개변수 time은 분단위로 입력받음
	int ans;

	ans = time / 60;

	return ans;
};

int ReturnMinute(int time) { // 분 반환 함수
	int ans;

	ans = time % 60;

	return ans;

};

int Return24Rate(int time) { // 24시간 비율 반환 함수
	int ans;
	int One_day = 1440;

	ans = (double)time / (double)One_day * (double)100;

	return ans;
}


//####
void DailyChart::Daily_statistics(vector<string>* db, string date_data) { // 일간 통계 함수.

	int cat[8] = { 0, }; // 각 카테고리별 총 시간 변수, 초기화 해줌.
	int count = 0; // 통계 출력부분이 처음인지 아닌지 구별하는 변수
	/* 인자 설명

	vector<string> db : DB 정보 포인터 인자.

	string date_data : 처음에 입력 받을 날짜 정보 2020-03-20 이런 string 형태로 받았음.

	*/

	vector<string> DB_test;

	int D_year = 0, D_month = 0, D_day = 0; // date_data에서 입력 받은 년월일을 할당할 변수

	All_Database.clear();

	Daily_DB_Read(&All_Database); // 현재 전체 DB 추출

	stringstream stream_test;
	stream_test.str(date_data);

	stream_test >> D_year >> D_month >> D_day; // 년월일 구분
	stream_test.clear();



	D_year = abs(D_year);
	D_month = abs(D_month);
	D_day = abs(D_day);



	printf("********************%d년 %d월 %d일의 통계결과 ***********************\n\n", D_year, D_month, D_day);

	Daily_DB_Search(db, date_data, 2); // 해당 일자만 추출해서 DB 업데이트

	for (unsigned int i = 0; i < (*db).size(); i++) {
		stream_test.clear();
		stream_test.str((*db)[i]);

		stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
			cartegory >> detail_chart; // 클래스 인자 구분

		if (cartegory == "수면") {
			cat[0] += btw_time;
			continue;
		}
		else if (cartegory == "식사") {
			cat[1] += btw_time;
			continue;
		}
		else if (cartegory == "수업") {
			cat[2] += btw_time;
			continue;
		}
		else if (cartegory == "공부") {
			cat[3] += btw_time;
			continue;
		}
		else if (cartegory == "운동") {
			cat[4] += btw_time;
			continue;
		}
		else if (cartegory == "취미") {
			cat[5] += btw_time;
			continue;
		}
		else if (cartegory == "아르바이트") {
			cat[6] += btw_time;
			continue;
		}
		else if (cartegory == "기타") {
			cat[7] += btw_time;
			continue;
		}
		break;
	}

	//	printf("\n\n******************************************************************************\n\n");

	for (int i = 0; i < 8; i++) {
		if (cat[i] == 0) { // 해당 카테고리에 투자한 시간이 없으면
			//넘어감
		}
		else {
			switch (i) {
			case 0: // 1. 수면
				printf("수면 : %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 1: // 2. 식사
				printf("식사 : %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 2: // 3. 수업
				printf("수업 : %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 3: // 4. 공부
				printf("공부 : %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 4: // 5. 운동
				printf("운동 : %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 5: // 6. 취미
				printf("취미 : %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 6: // 7. 아르바이트
				printf("아르바이트 : %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			default: // 8. 기타 
				printf("기타 : %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			}
		}
	}


	printf("\n\n******************************************************************************\n\n");

	//24시간 기준 각 활동 비율 출력 ####

	printf("24시간기준 각 활동 비율 : ");

	for (int i = 0; i < 8; i++) {
		if (cat[i] == 0) { // 해당 카테고리에 투자한 시간이 없으면
			//넘어감
		}
		else {

			switch (i) {
			case 0: // 1. 수면
				printf("수면(%d%%)", Return24Rate(cat[i]));
				break;
			case 1: // 2. 식사
				if (count != 0)
					printf(", ");
				printf("식사(%d%%)", Return24Rate(cat[i]));
				break;
			case 2: // 3. 수업
				if (count != 0)
					printf(", ");
				printf("수업(%d%%)", Return24Rate(cat[i]));
				break;
			case 3: // 4. 공부
				if (count != 0)
					printf(", ");
				printf("공부(%d%%)", Return24Rate(cat[i]));
				break;
			case 4: // 5. 운동
				if (count != 0)
					printf(", ");
				printf("운동(%d%%)", Return24Rate(cat[i]));
				break;
			case 5: // 6. 취미
				if (count != 0)
					printf(", ");
				printf("취미(%d%%)", Return24Rate(cat[i]));
				break;
			case 6: // 7. 아르바이트
				if (count != 0)
					printf(", ");
				printf("아르바이트(%d%%)", Return24Rate(cat[i]));
				break;
			default: // 8. 기타 하루 통계
				if (count != 0)
					printf(", ");
				printf("기타(%d%%)", Return24Rate(cat[i]));
				break;
			}
			count++;
		}

	}

	printf("\n\n******************************************************************************\n\n");
	printf("아무 키나 누르면 메뉴화면으로 돌아갑니다.\n");
	
	

}


void menu_2() {  //일간 통계
	rewind(stdin); // 잉여 키값 초기화
	reopen++;
	system("cls");
	printf("************************************************************************************");
	printf("\n\n");
	printf("\t\t\t\t일간통계");
	printf("\n\n");
	printf("************************************************************************************");
	printf("\n\n");

	string Date_2;
	DailyChart D_2; // 클래스 선언
	int error_check; // 오류 변수

	vector<string> Database_2; // 메뉴 2에서만 사용할 데이터 베이스


	while (1) {
		printf("Insert Year/Month/Date : ");

		std::getline(cin, Date_2);

		// 날짜 오류 확인
		error_check = DateCheck(&Date_2);
		if (0 == error_check) { // 오류가 없으면
			break;
		}
		else { //오류가 있으면 ####
			/*각 리턴 값 별로 오류 설명*/
		}

	}

	D_2.Daily_statistics(&Database_2, Date_2);



	rewind(stdin); // 잉여 키값 초기화 
	while (!kbhit()) // 아무키 입력 받으면 종료되는 것
	{

	}

}

int CheckIntercalation(int year) { // 윤년 계산기. 윤년이면 1리턴, 아니면 0리턴
	int temp;

	/* 윤년 구하는 법
	1. 4의 배수인 해는 윤년으로 한다.
	2. 그러나 100의 배수인 해는 윤년에서 뺀다.
	3. 그러나 400의 배수인 해는 도로 윤년으로 넣는다.
	4. 4000(일부에선 3200을 제안)의 배수인 해는 윤년이 아니다.
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
void DailyChart::Monthly_statistics(vector<string>* db, string date_data) {  // 월간 통계 함수.
	int cat[8] = { 0, }; // 각 카테고리별 총 시간 변수, 초기화 해줌.

	/* 인자 설명

	vector<string> db : DB 정보 포인터 인자.

	string date_data : 처음에 입력 받을 날짜 정보 2020-03 이런 string 형태로 받았음.

	*/


	int D_year = 0, D_month = 0; // date_data에서 입력 받은 년월일을 할당할 변수
	int count = 0;

	All_Database.clear();
	Daily_DB_Read(&All_Database); // 현재 전체 DB 추출

	stringstream stream_test;
	stream_test.str(date_data);

	stream_test >> D_year >> D_month; // 년월 구분
	stream_test.clear();

	D_year = abs(D_year);
	D_month = abs(D_month);

	vector<string> DB_test;

	printf("********************%d년 %d월의 통계결과 ***********************\n\n", D_year, D_month);

	//db->clear();
	Daily_DB_Search(&DB_test, date_data, 0); // 해당 일자만 추출해서 DB 업데이트



	// 총 시간 계산

	for (unsigned int i = 0; i < (DB_test).size(); i++) {
		stream_test.clear();
		stream_test.str((DB_test)[i]);

		stream_test >> num >> year >> month >> day >> start_time >> end_time >> btw_time >>
			cartegory >> detail_chart; // 클래스 인자 구분

		if (cartegory == "수면") {
			cat[0] += btw_time;
			continue;
		}
		else if (cartegory == "식사") {
			cat[1] += btw_time;
			continue;
		}
		else if (cartegory == "수업") {
			cat[2] += btw_time;
			continue;
		}
		else if (cartegory == "공부") {
			cat[3] += btw_time;
			continue;
		}
		else if (cartegory == "운동") {
			cat[4] += btw_time;
			continue;
		}
		else if (cartegory == "취미") {
			cat[5] += btw_time;
			continue;
		}
		else if (cartegory == "아르바이트") {
			cat[6] += btw_time;
			continue;
		}
		else if (cartegory == "기타") {
			cat[7] += btw_time;
			continue;
		}
		break;
	}
	// 각 카테고리별 평균 시간 구하기

	if (D_month == 1 || D_month == 3 || D_month == 5 || D_month == 7 || D_month == 8 || D_month == 10 || D_month == 12) { // 31일이 있는 달
		cat[0] /= 31;
		cat[1] /= 31;
		cat[2] /= 31;
		cat[3] /= 31;
		cat[4] /= 31;
		cat[5] /= 31;
		cat[6] /= 31;
		cat[7] /= 31;



	}
	else if (D_month == 2) { // 28일까지 + 윤달 고려
		if (CheckIntercalation(D_year)) { // 윤달일 경우
			cat[0] /= 29;
			cat[1] /= 29;
			cat[2] /= 29;
			cat[3] /= 29;
			cat[4] /= 29;
			cat[5] /= 29;
			cat[6] /= 29;
			cat[7] /= 29;
		}
		else { // 아닐 경우
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
	else { // 나머지 30일만 있는 달
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
		if (cat[i] == 0) { // 해당 카테고리에 투자한 시간이 없으면
			//넘어감
		}
		else {
			switch (i) {
			case 0: // 1. 수면
				printf("평균 수면 시간: %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 1: // 2. 식사
				printf("평균 식사 시간: %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 2: // 3. 수업
				printf("평균 수면 시간: %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 3: // 4. 공부
				printf("평균 공부 시간 : %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 4: // 5. 운동
				printf("평균 운동 시간: %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 5: // 6. 취미
				printf("평균 취미 시간: %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			case 6: // 7. 아르바이트
				printf("평균 아르바이트 시간: %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			default: // 8. 기타 
				printf("평균 기타 시간: %d시간 %d분\n\n", ReturnHour(cat[i]), ReturnMinute(cat[i]));
				break;
			}
		}
	}


	printf("\n\n******************************************************************************\n\n");

	//24시간 기준 각 활동 비율 출력 ####

	printf("24시간기준 각 활동 비율 : ");

	for (int i = 0; i < 8; i++) {
		if (cat[i] == 0) { // 해당 카테고리에 투자한 시간이 없으면
			//넘어감
		}
		else {

			switch (i) {
			case 0: // 1. 수면
				printf("수면(%d%%)", Return24Rate(cat[i]));
				break;
			case 1: // 2. 식사
				if (count != 0)  // 처음 출력하는 게 아니면 ','가 필요하프로 출력한다.
					printf(", ");
				printf("식사(%d%%)", Return24Rate(cat[i]));
				break;
			case 2: // 3. 수업
				if (count != 0)
					printf(", ");
				printf("수업(%d%%)", Return24Rate(cat[i]));
				break;
			case 3: // 4. 공부
				if (count != 0)
					printf(", ");
				printf("공부(%d%%)", Return24Rate(cat[i]));
				break;
			case 4: // 5. 운동
				if (count != 0)
					printf(", ");
				printf("운동(%d%%)", Return24Rate(cat[i]));
				break;
			case 5: // 6. 취미
				if (count != 0)
					printf(", ");
				printf("취미(%d%%)", Return24Rate(cat[i]));
				break;
			case 6: // 7. 아르바이트
				if (count != 0)
					printf(", ");
				printf("아르바이트(%d%%)", Return24Rate(cat[i]));
				break;
			default: // 8. 기타 
				if (count != 0)
					printf(", ");
				printf("기타(%d%%)", Return24Rate(cat[i]));
				break;
			}
			count++;
		}

	}

	printf("\n\n******************************************************************************\n\n");
	printf("아무 키나 누르면 메뉴화면으로 돌아갑니다.\n");

	
}


//#### (김훈기) cf) (구)insert_month
int DateCheck2(string* search_date) { // 년도, 월 만 받을때 오류조사
	/* date 를 표준 형태로 바꾸는 과정도 포함시킬 것*/
	//2020/100 , 2020/001 , 001, 즉 년도o 월 년도x 월  1,3,5,6
	int o1, o2, o3, o4, z1,z2,z3,z4,z5 ,si, si2,l;
	
	si = sign((char*)(*search_date).c_str());
	si2 = signmiss((char*)(*search_date).c_str());
	l = last((char*)(*search_date).c_str());
	z5 = zerogu4((char*)(*search_date).c_str()); // 맨앞 
	z1= zerogu1((char*)(*search_date).c_str());
	z2 = zerogu5((char*)(*search_date).c_str());


	if (si == 2) {
		printf("잘못된 문자를 입력하셨습니다. 날짜를 다시 입력해주세요. \n");
		return 1;
	}
	if ((*search_date)[0] == '.' || (*search_date)[0] == '/' || (*search_date)[0] == ' ' || (*search_date)[0] == '-') {
		printf("형식에 맞지 않습니다. 날짜를 다시 입력해주세요. \n");
		return 1;
	}
	if (si2 == 2) {
		printf("문자열을 한번에 두 번 입력했습니다. 날짜를 다시 입력해주세요. \n");
		return 1;
	}
	if (l == 2) {
		printf("형식에 맞지 않습니다. 날짜를 다시 입력해주세요. \n");
		return 1;
	}


	char* oper = strchr((char*)search_date->c_str(), '.');
	char* oper1 = strchr((char*)search_date->c_str(), '/');
	char* oper2 = strchr((char*)search_date->c_str(), ' ');
	char* oper3 = strchr((char*)search_date->c_str(), '-'); //요것들 찾아서 위치반환
	char* num = strtok((char*)search_date->c_str(), " -/.");

	int nums[100];
	int i = 0; ; int j = 0;
	char nums2[100];

	if (oper != NULL || oper1 != NULL || oper2 != NULL || oper3 != NULL) { // 구분문자가 있는지 없는지 먼저 구분
	  //구분문자 빼고 숫자 저장
		while (num != NULL) { //배열에 숫자들 저장
			nums[i++] = atoi(num);
			num = strtok(NULL, " -/.");
		}//  년도까지 i는 3반환 4개부턴 잘못쓰
		if (i > 2) {
			printf("문자열이 올바르지 않습니다. 날짜를 다시 입력해주세요.\n");
			return 1;
		}
		else {
			if (i == 2) {
				if (nums[0] >= 2000 && nums[0] <= 2999) {
					if (nums[1] >= 1 && nums[1] <= 12) {
						//printf("날짜 넣기1");
						if (z1 == 2) {
							printf("선행문자 ‘0’을 너무 많이 입력했습니다. 날짜를 다시 입력해주세요.\n");
							return 1;
						}
						string y = to_string(nums[0]);
						string m = to_string(nums[1]);
						*search_date = y + '-' + m;
						return 0;
					}
					else {
						printf("달의 입력범위가 잘못되었습니다. 날짜를 다시 입력해주세요.\n");
						return 1;
					}
				}
				else {
					printf("년도 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
					return 1;
				}
			}//년도 까지 다입력한거
			else {
				printf("형식에 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
				return 1;
			}//일이나 월만입력한거 , 구분문자 있을경우
		}// printf("구분문자잇음");
	}// 구분문자있는거 종료
	else {// printf("구분문자없음");
	   //먼저 년도입력, 년도 안입력으로 구분 4자리 2자리 2자리
		int i = 0;
		int num = strlen(search_date->c_str());
		if (num == 6) {
			//year1,month1,date1
			nums[0] = year1((char*)search_date->c_str()); nums[1] = month1((char*)search_date->c_str());
			if ((nums[0] >= 2000 && nums[0] <= 2999)) {
				if (nums[1] >= 1 && nums[1] <= 12) {
					//printf("날짜입력3");
					string y = to_string(nums[0]);
					string m = to_string(nums[1]);
					*search_date = y + '-' + m;
					return 0;
				}
				else {
					printf("월 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
					return 1;
				}
			}
			else {
				printf("년도 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
				return 1;
			}
		}
		else if (num == 2) { // month2 , date 2
			time_t base_time = time(NULL);
			struct tm* newtime = localtime(&base_time);
			int t = newtime->tm_year + 1900;
			nums[0] = month2((char*)search_date->c_str());
			if (nums[0] >= 1 && nums[0] <= 12) {
				//printf("날짜넣기4");
				string y = to_string(t);
				string m = to_string(nums[0]);
				*search_date = y + '-' + m;
				return 0;
			}
			else {
				printf("월 범위가 맞지 않습니다. 날짜를 다시 입력해주세요.\n");
				return 1;
			}
		}
		else {
			printf("형식에 맞지 않습니다. 날짜를 다시 입력해주세요. \n");
			return 1;
		}
	}

	return 0;
}


void menu_3() { // 월간 통계
	rewind(stdin); // 잉여 키값 초기화
	reopen++;
	system("cls");
	printf("************************************************************************************");
	printf("\n\n");
	printf("\t\t\t*월간 통계*");
	printf("\n\n");
	printf("************************************************************************************");
	printf("\n\n");

	string Date_3;
	DailyChart D_3; // 클래스 선언
	int error_check; // 오류 변수

	vector<string> Database_3; // 메뉴 2에서만 사용할 데이터 베이스


	while (1) {
		printf("Insert Year/Month: ");

		std::getline(cin, Date_3);

		// 날짜 오류 확인
		error_check = DateCheck2(&Date_3);
		if (0 == error_check) // 오류가 없으면
		{
			break;
		}
		else //오류가 있으면 ####
		{
			/*각 리턴 값 별로 오류 설명*/
		}

	}

	D_3.Monthly_statistics(&Database_3, Date_3);



	rewind(stdin); // 잉여 키값 초기화 
	while (!kbhit()) // 아무키 입력 받으면 종료되는 것
	{

	}

}

void menu_4() { // 도움말 (강민호)
	rewind(stdin); // 잉여 키값 초기화
	system("cls");
	reopen++;
	vector<string> help_DB;
	//DailyChart D_4;

	ifstream readfile; // 읽어들일 파일 변수



	readfile.open("Help.txt");


	if (readfile.is_open()) {
		while (!readfile.eof())
		{
			string str;
			std::getline(readfile, str); // DB.txt 파일을 한줄씩 읽어옴


			(help_DB).push_back(str); // 읽어온 정보를 db 벡터인자에 차곡차곡 저장함.

		}
	}

	readfile.close();

	for (unsigned int i = 0; i < (help_DB).size(); i++) {// db 크기 만큼 모든 행을 출력함.
		cout << (help_DB)[i] << endl;

	}

	
	rewind(stdin); // 잉여 키값 초기화 
	while (!kbhit()) // 아무키 입력 받으면 종료되는 것
	{

	}
	//getch(); // 아무 키 입력하면 종료됨.
}

void menu_5() {// 종료 (조준영)
	rewind(stdin); // 잉여 키값 초기화
	system("cls");
	printf("******************************************************************************\n\n");
	printf("\n\n\n");
	printf("\t\t\t\tBye\n");
	printf("\n\n");
	printf("\n\n******************************************************************************\n\n");
	printf("이 창을 닫으려면 아무 키나 누르세요...\n");

	rewind(stdin); // 잉여 키값 초기화 

	while (!kbhit()) // 아무키 입력 받으면 종료되는 것
	{

	}
	
	
}


int main(void) {


	DailyChart D; // 클래스 선언


	string want_data; // (찾고 싶은 정보를) 입력 받을 string 변수

	All_Database.clear(); // 벡터 배열 초기화



	string menu_number;


	// DB.txt 체크 (강민호)
	int n;
	string err_text;
	if (db_check == 0)
	{
		if (D.Daily_DB_Check(&err_text, &n)) { // 우리의 DB가 아니면 1이 반환되어서 if문이 진행됨.
			printf("DB가 (%s/입력양식)이 맞지 않아 손상되었습니다. DB.txt의 (%d 번)을 수정하십시오.\n", err_text.c_str(), n);
			Sleep(2000); // DB가 비정상일때 출력되는 문구를 보여주려고 넣음
			return 0;
		}
		else { // 우리 DB가 맞으면 0이 반환되어서 else문이 진행됨.
			printf("DB가 정상입니다!\n");

		}

		Sleep(2000); // DB가 정상일때 출력되는 문구를 보여주려고 넣음

		// DB. txt 에서 정보 끌어오기 (초기 DB 세팅)

		All_Database.clear();

		D.Daily_DB_Read(&All_Database); // All_Database 벡터 배열에다가 읽어온 DB 정보를 담음.
	
		db_check++;
	}

	char menu_num[1000];
	
	// 메인 화면 (조준영)


	while (1) {
		rewind(stdin); // 잉여 키값 초기화 : 비주얼 스튜디오에서만 지원되는 함수라 다른 곳에선 못쓴다.(참고)
		system("cls");
		printf("******************************************************************************\n\n");
		printf("Hello. We are Team2\n\n");
		printf("Captain : 이일진\n");
		printf("Member : 강민호, 김훈기, 이정수, 조준영\n");
		printf("Subject : 전공기초 프로젝트1\n");
		printf("Professor : 차리서\n");
		printf("\n\n******************************************************************************\n\n");
		printf("Please Select Menu Number\n\n");
		printf("1. 일정입력, 수정, 삭제\n");
		printf("2. 카테고리 추가, 수정, 삭제\n");
		printf("3. 일간통계\n");
		printf("4. 월간통계\n");
		printf("5. 사용방법\n");
		printf("6. 종료\n");

		while (1) {
			
			printf("Insert Number : ");

			
			//gets_s(menu_num);

			//menu_number = menu_num;

			//cin.ignore();
			//getline(cin, menu_number);

			cin >>menu_number;

			//rewind(stdin); // 잉여 키값 초기화 
			//kbhit();// getchar();
			/*
			if (menu_number.length() > 1 || menu_number.length() == 0)
			{
				printf("\n잘못된 입력입니다. 1에서5 사이의 자연수를 입력해주세요\n");
				rewind(stdin); // 잉여 키값 초기화 
				while (!kbhit()) // 아무키 입력 받으면 종료되는 것
				{

				}
				rewind(stdin); // 잉여 키값 초기화 
				//cin.ignore();
				break;
			}*/


			/* cf) C++의 String 타입 말고 C언어의 배열로만 받고 싶때

			설명 :

				  만일 cpp의 string 헤더를 쓰지 않고 C언어로만 만들고 싶을 때

				  배열 타입으로 받아들이려면

				  char* arr[] = new char[]; 이런식으로 동적할당 정의. (메모리 낭비 방지, 유동적인 크기를 받아들이기 위함)

				  scanf로 arr에 입력을 받은 다음

				  arr[1] 일일이 비교를 해서

				  (arr[0]만 있다 = 1의자리수
				  arr[1]가 있다? = 자리수가 2개 이상임을 이용)

				  있으면 두 자리수 이상 숫자를 입력 받은거라 else (예외처리)하고, 없으면

				  한자리수만 있는 놈이라 arr[0] == '1','2','3','4','5'' else 이렇게 처리하면 됨.

				  참고만 할 것.

			 */


			if (menu_number == "1") { // 일정 입력, 수정, 삭제 메뉴

				menu_1();
				break;
			}
			else if (menu_number == "2") { // 일간 통계 메뉴

				menu_2();
				//cin.ignore();
				break;
			}
			else if (menu_number == "3") { // 월간 통계 메뉴

				menu_3();
				
				break;
			}
			else if (menu_number == "4") { // 사용방법 메뉴

				menu_4();
				break;
			}
			else if (menu_number == "5") { // 종료하기

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
			    getch(); // 아무 키 입력하면 종료됨.
				main();*/

				printf("\n잘못된 입력입니다. 1에서5 사이의 자연수를 입력해주세요\n");
				while (!kbhit()) // 아무키 입력 받으면 종료되는 것
				{

				}
				rewind(stdin); // 잉여 키값 초기화 

				//getch(); // 아무 키 입력하면 종료됨.
				break;
			}
		}
	}



	return 0;
}