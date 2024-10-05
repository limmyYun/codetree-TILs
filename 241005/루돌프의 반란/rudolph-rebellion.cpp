일반 연습 / 기출 문제







아이디	결과	수행 시간	메모리	언어	제출 일시
yhly97	틀렸습니다 (테스트케이스 확인)	5ms	0MB	C++14	1분 전
yhly97	틀렸습니다 (테스트케이스 확인)	6ms	0MB	C++14	6달 전
yhly97	런타임 에러	6ms	0MB	C++14	6달 전
yhly97	런타임 에러	6ms	0MB	C++14	6달 전
yhly97	틀렸습니다 (테스트케이스 확인)	6ms	0MB	C++14	6달 전
페이지당 제출 수

10
1–5 of 5

C++14

라이트 모드
444142433839403637333435303132282945464748495051525354555657585960
			movr = mr + dy4[direction];
			movc = mc + dx4[direction];
			dfs(mr, mc, movr, movc, direction, flag);
		}
	}

	int idx = map[r][c]-1;
	//cout << "before update : idx : " << idx << endl;
	map[mr][mc] = idx+1;
	map[r][c] = 0;

2426void dfs(int r, int c, int mr, int mc, int direction, int flag) {    if (mr < 0 || mc < 0 || mr >= N || mc >= N) {

테스트 케이스

5 7 4 2 2
3 2
1 1 3
2 3 5
3 5 1
4 4 4
정답

11 6 2 7 

나의 출력

11 6 2 7 

public

클립보드에 복사 되었습니다.

Cannot edit in read-only editor