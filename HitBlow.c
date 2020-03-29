#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* マクロ定義 */
#define STATIC1_ID 201
#define STATIC2_ID 202
#define STATIC3_ID 203 
#define STATIC4_ID 204
#define STATIC5_ID 205
#define STATIC6_ID 206
#define STATIC1_TEXT "範囲：0～9 重複：なし　回数：5回"
#define EDIT1_ID 301
#define EDIT2_ID 302
#define EDIT3_ID 303
#define BUTTON1_CAPTION "チャレンジ"
#define BUTTON2_CAPTION "リセット"
#define BUTTON1_ID 101
#define BUTTON2_ID 102
#define ANSWER_TIMES 5

/* プロトタイプ宣言 */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int challenge();
int reset();
int CreateWnd();
int CreateObj();
int CreateAnsNum();
int JudgeNum();

// 型にしちゃうと数字1～3の走査がforループでできなくなっちゃうけど、
// 型の勉強をしたかったからこうした。
typedef struct {
    int num1;
    int num2;
    int num3;
    int hit;
    int blow;
} NUM;

/* グローバル変数 */
WNDCLASS wc; // ウィンドウクラス
MSG msg; // メッセージ
HINSTANCE hIns;
HWND hwnd;
HWND edit1;
HWND edit2;
HWND edit3;
HWND static1;
HWND static2;
HWND static3;
HWND static4;
HWND static5;
HWND static6;
HWND button1;
HWND button2;
NUM pastNum[ANSWER_TIMES];
NUM ansNum;
int ansCnt; // 回答した回数

/*******************************
 * メイン関数（一番最初に呼ばれる）
 *******************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    int i;
    hIns = hInstance;

    // 過去の回答初期化
    for (i = 0; i < ANSWER_TIMES; i++)
    {
        pastNum[i].num1 = 0;
        pastNum[i].num2 = 0;
        pastNum[i].num3 = 0;
        pastNum[i].hit = 0;
        pastNum[i].blow = 0;
    }
    ansCnt = 0;

    // 正解ナンバー作成
    CreateAnsNum();

    // ウィンドウ作成
    CreateWnd();

    // オプジェクト作成
    CreateObj();

    return msg.wParam;
}

/*******************************
 * ウィンドウプロシージャ
 *******************************/
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    int retBtn;

    switch(msg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_COMMAND:
            switch(LOWORD(wp))
            {
                case BUTTON1_ID: // チャレンジボタン押下時
                    challenge();
                    break;
                case BUTTON2_ID: // リセットボタン押下時
                    retBtn = MessageBox(hwnd, "リセットしても\nよろしいですか？", "リセット", MB_YESNO | MB_ICONQUESTION);
                    if(retBtn == IDYES)
                    {
                        reset();
                    }
                    break;
            }
            return 0;
        case WM_CREATE:
            break;
    }

    return DefWindowProc(hwnd, msg, wp, lp);
}

/*******************************
 * チャレンジボタン押下時
 *******************************/
int challenge()
{
    int ret;
    char strText[32768];
    PSTR retText;
    int i;
    HWND tmp = NULL;
    PSTR tmpNum;

    ansCnt++;

    // 回答を得る
    tmpNum = malloc(32768);
    memset(tmpNum, '\0', sizeof(tmpNum));
    GetWindowText(edit1, tmpNum, 32768);
    pastNum[ansCnt - 1].num1 = atoi(tmpNum);
    memset(tmpNum, '\0', sizeof(tmpNum));
    GetWindowText(edit2, tmpNum, 32768);
    pastNum[ansCnt - 1].num2 = atoi(tmpNum);
    memset(tmpNum, '\0', sizeof(tmpNum));
    GetWindowText(edit3, tmpNum, 32768);
    pastNum[ansCnt - 1].num3 = atoi(tmpNum);
    free(tmpNum);

    // 回答と正解を突き合わせる
    JudgeNum();

    // 画面の再描画
    switch(ansCnt)
    {
        case 0:
            // あり得ない
            break;
        case 1:
            tmp = static2;
            break;
        case 2:
            tmp = static3;
            break;
        case 3:
            tmp = static4;
            break;
        case 4:
            tmp = static5;
            break;
        case 5:
            tmp = static6;
            break;
        default:
            break;
    }

    memset(strText, '\0', sizeof(strText));
    sprintf(strText, "%d回目　%d　%d　%d　%dHit　%dBlow", ansCnt, pastNum[ansCnt - 1].num1, pastNum[ansCnt - 1].num2, pastNum[ansCnt - 1].num3, pastNum[ansCnt - 1].hit, pastNum[ansCnt - 1].blow);

    SetWindowText(tmp, strText);

    if(pastNum[ansCnt - 1].hit == 3)
    {
        MessageBox(hwnd, "お見事!!!!", "正解", MB_OK);
        ShowWindow(button1, SW_HIDE);
    }
    else if(ansCnt == ANSWER_TIMES)
    {
        memset(strText, '\0', sizeof(strText));
        sprintf(strText, "正解は　%d　%d　%d　でした。", ansNum.num1, ansNum.num2, ansNum.num3);
        MessageBox(hwnd, strText, "残念", MB_OK);
        ShowWindow(button1, SW_HIDE);
    }

    return 0;
}

/*******************************
 * リセットボタン押下時
 *******************************/
int reset()
{
    int i;

    // 過去の回答初期化
    for (i = 0; i < ANSWER_TIMES; i++)
    {
        pastNum[i].num1 = 0;
        pastNum[i].num2 = 0;
        pastNum[i].num3 = 0;
        pastNum[i].hit = 0;
        pastNum[i].blow = 0;
    }
    ansCnt = 0;

    // 正解ナンバー作成
    CreateAnsNum();

    // 画面初期化
    SetWindowText(static2, "");
    SetWindowText(static3, "");
    SetWindowText(static4, "");
    SetWindowText(static5, "");
    SetWindowText(static6, "");
    SetWindowText(edit1, "");
    SetWindowText(edit2, "");
    SetWindowText(edit3, "");

    if(IsWindowVisible(button1) == 0)
    {
        ShowWindow(button1, SW_SHOW);
    }

    return 0;
}

/*******************************
 * ウィンドウ作成
 *******************************/
int CreateWnd()
{
    // メインウィンドウクラスの定義
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hIns;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "TEST";

    // ウィンドウクラスの登録
    if(!RegisterClass(&wc))
    {
        return -1;
    }

    // ウィンドウ作成開始
    hwnd = CreateWindow("Test", "■Hit & Blow■", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hIns, NULL);
    if(hwnd == NULL)
    {
        return -1;
    }

    return 0;
}

/*******************************
 * オブジェクト作成
 *******************************/
int CreateObj()
{
    // ボタン1
    // 押されると親ウィンドウのウィンドウプロシージャにWM_COMMANDを送る
    button1 = CreateWindow("BUTTON", BUTTON1_CAPTION, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 400, 100, 50, hwnd, (HMENU)BUTTON1_ID, hIns, NULL);

    // ボタン2
    button2 = CreateWindow("BUTTON", BUTTON2_CAPTION, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 600, 400, 100, 50, hwnd, (HMENU)BUTTON2_ID, hIns, NULL);

    // スタティック
    static1 = CreateWindow("STATIC", STATIC1_TEXT, WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 100, 300, 30, hwnd, (HMENU)STATIC1_ID, hIns, NULL);

    // スタティック
    static2 = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 130, 300, 30, hwnd, (HMENU)STATIC2_ID, hIns, NULL);

    // スタティック
    static3 = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 160, 300, 30, hwnd, (HMENU)STATIC3_ID, hIns, NULL);

    // スタティック
    static4 = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 190, 300, 30, hwnd, (HMENU)STATIC4_ID, hIns, NULL);

    // スタティック
    static5 = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 220, 300, 30, hwnd, (HMENU)STATIC5_ID, hIns, NULL);

    // スタティック
    static6 = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 250, 300, 30, hwnd, (HMENU)STATIC6_ID, hIns, NULL);

    // エディット
    edit1 = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 440, 340, 15, 26, hwnd, (HMENU)EDIT1_ID, hIns, NULL);

    // エディット
    edit2 = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 490, 340, 15, 26, hwnd, (HMENU)EDIT2_ID, hIns, NULL);

    // エディット
    edit3 = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 540, 340, 15, 26, hwnd, (HMENU)EDIT3_ID, hIns, NULL);

    // メッセージループ
    while(GetMessage(&msg, NULL, 0, 0))
    {
        // メッセージをウィンドウプロシージャにも渡す
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

/*******************************
 * 正解の作成
 *******************************/
int CreateAnsNum()
{
    int r;
    int cnt = 0;
    ansNum.num1 = -1;
    ansNum.num2 = -1;
    ansNum.num3 = -1;

    while(cnt < 3)
    {
        srand(time(NULL));
        r = rand() % 10;

        switch(cnt)
        {
            case 0:
                ansNum.num1 = r; // 1個目に入れる
                cnt++;
                break;
            case 1:
                if(ansNum.num1 != r) // 1個目と一致しない
                {
                    ansNum.num2 = r; // 2個目に入れる
                    cnt++;
                }
                break;
            case 2:
                if((ansNum.num1 != r) && (ansNum.num2 != r)) // 1個目、2個目と一致しない
                {
                    ansNum.num3 = r; // 3個目に入れる
                    cnt++;
                }
                break;
        }
    }

    return 0;
}

/*******************************
 * 正解の判定
 *******************************/
int JudgeNum()
{
    int i;
    int j;
    int ans[3];
    int inp[3];

    ans[0] = ansNum.num1;
    ans[1] = ansNum.num2;
    ans[2] = ansNum.num3;

    inp[0] = pastNum[ansCnt - 1].num1;
    inp[1] = pastNum[ansCnt - 1].num2;
    inp[2] = pastNum[ansCnt - 1].num3;

    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            if(ans[i] == inp[j] && i == j)
            {
                pastNum[ansCnt - 1].hit++;
            }
            else if(ans[i] == inp[j])
            {
                pastNum[ansCnt - 1].blow++;
            }
        }
    }

    return 0;
}
