#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* �}�N����` */
#define STATIC1_ID 201
#define STATIC2_ID 202
#define STATIC3_ID 203 
#define STATIC4_ID 204
#define STATIC5_ID 205
#define STATIC6_ID 206
#define STATIC1_TEXT "�͈́F0�`9 �d���F�Ȃ��@�񐔁F5��"
#define EDIT1_ID 301
#define EDIT2_ID 302
#define EDIT3_ID 303
#define BUTTON1_CAPTION "�`�������W"
#define BUTTON2_CAPTION "���Z�b�g"
#define BUTTON1_ID 101
#define BUTTON2_ID 102
#define ANSWER_TIMES 5

/* �v���g�^�C�v�錾 */
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int challenge();
int reset();
int CreateWnd();
int CreateObj();
int CreateAnsNum();
int JudgeNum();

// �^�ɂ����Ⴄ�Ɛ���1�`3�̑�����for���[�v�łł��Ȃ��Ȃ����Ⴄ���ǁA
// �^�̕׋����������������炱�������B
typedef struct {
    int num1;
    int num2;
    int num3;
    int hit;
    int blow;
} NUM;

/* �O���[�o���ϐ� */
WNDCLASS wc; // �E�B���h�E�N���X
MSG msg; // ���b�Z�[�W
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
int ansCnt; // �񓚂�����

/*******************************
 * ���C���֐��i��ԍŏ��ɌĂ΂��j
 *******************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow)
{
    int i;
    hIns = hInstance;

    // �ߋ��̉񓚏�����
    for (i = 0; i < ANSWER_TIMES; i++)
    {
        pastNum[i].num1 = 0;
        pastNum[i].num2 = 0;
        pastNum[i].num3 = 0;
        pastNum[i].hit = 0;
        pastNum[i].blow = 0;
    }
    ansCnt = 0;

    // �����i���o�[�쐬
    CreateAnsNum();

    // �E�B���h�E�쐬
    CreateWnd();

    // �I�v�W�F�N�g�쐬
    CreateObj();

    return msg.wParam;
}

/*******************************
 * �E�B���h�E�v���V�[�W��
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
                case BUTTON1_ID: // �`�������W�{�^��������
                    challenge();
                    break;
                case BUTTON2_ID: // ���Z�b�g�{�^��������
                    retBtn = MessageBox(hwnd, "���Z�b�g���Ă�\n��낵���ł����H", "���Z�b�g", MB_YESNO | MB_ICONQUESTION);
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
 * �`�������W�{�^��������
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

    // �񓚂𓾂�
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

    // �񓚂Ɛ�����˂����킹��
    JudgeNum();

    // ��ʂ̍ĕ`��
    switch(ansCnt)
    {
        case 0:
            // ���蓾�Ȃ�
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
    sprintf(strText, "%d��ځ@%d�@%d�@%d�@%dHit�@%dBlow", ansCnt, pastNum[ansCnt - 1].num1, pastNum[ansCnt - 1].num2, pastNum[ansCnt - 1].num3, pastNum[ansCnt - 1].hit, pastNum[ansCnt - 1].blow);

    SetWindowText(tmp, strText);

    if(pastNum[ansCnt - 1].hit == 3)
    {
        MessageBox(hwnd, "������!!!!", "����", MB_OK);
        ShowWindow(button1, SW_HIDE);
    }
    else if(ansCnt == ANSWER_TIMES)
    {
        memset(strText, '\0', sizeof(strText));
        sprintf(strText, "�����́@%d�@%d�@%d�@�ł����B", ansNum.num1, ansNum.num2, ansNum.num3);
        MessageBox(hwnd, strText, "�c�O", MB_OK);
        ShowWindow(button1, SW_HIDE);
    }

    return 0;
}

/*******************************
 * ���Z�b�g�{�^��������
 *******************************/
int reset()
{
    int i;

    // �ߋ��̉񓚏�����
    for (i = 0; i < ANSWER_TIMES; i++)
    {
        pastNum[i].num1 = 0;
        pastNum[i].num2 = 0;
        pastNum[i].num3 = 0;
        pastNum[i].hit = 0;
        pastNum[i].blow = 0;
    }
    ansCnt = 0;

    // �����i���o�[�쐬
    CreateAnsNum();

    // ��ʏ�����
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
 * �E�B���h�E�쐬
 *******************************/
int CreateWnd()
{
    // ���C���E�B���h�E�N���X�̒�`
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

    // �E�B���h�E�N���X�̓o�^
    if(!RegisterClass(&wc))
    {
        return -1;
    }

    // �E�B���h�E�쐬�J�n
    hwnd = CreateWindow("Test", "��Hit & Blow��", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hIns, NULL);
    if(hwnd == NULL)
    {
        return -1;
    }

    return 0;
}

/*******************************
 * �I�u�W�F�N�g�쐬
 *******************************/
int CreateObj()
{
    // �{�^��1
    // �������Ɛe�E�B���h�E�̃E�B���h�E�v���V�[�W����WM_COMMAND�𑗂�
    button1 = CreateWindow("BUTTON", BUTTON1_CAPTION, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 300, 400, 100, 50, hwnd, (HMENU)BUTTON1_ID, hIns, NULL);

    // �{�^��2
    button2 = CreateWindow("BUTTON", BUTTON2_CAPTION, WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 600, 400, 100, 50, hwnd, (HMENU)BUTTON2_ID, hIns, NULL);

    // �X�^�e�B�b�N
    static1 = CreateWindow("STATIC", STATIC1_TEXT, WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 100, 300, 30, hwnd, (HMENU)STATIC1_ID, hIns, NULL);

    // �X�^�e�B�b�N
    static2 = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 130, 300, 30, hwnd, (HMENU)STATIC2_ID, hIns, NULL);

    // �X�^�e�B�b�N
    static3 = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 160, 300, 30, hwnd, (HMENU)STATIC3_ID, hIns, NULL);

    // �X�^�e�B�b�N
    static4 = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 190, 300, 30, hwnd, (HMENU)STATIC4_ID, hIns, NULL);

    // �X�^�e�B�b�N
    static5 = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 220, 300, 30, hwnd, (HMENU)STATIC5_ID, hIns, NULL);

    // �X�^�e�B�b�N
    static6 = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_CENTER, 360, 250, 300, 30, hwnd, (HMENU)STATIC6_ID, hIns, NULL);

    // �G�f�B�b�g
    edit1 = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 440, 340, 15, 26, hwnd, (HMENU)EDIT1_ID, hIns, NULL);

    // �G�f�B�b�g
    edit2 = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 490, 340, 15, 26, hwnd, (HMENU)EDIT2_ID, hIns, NULL);

    // �G�f�B�b�g
    edit3 = CreateWindow("EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 540, 340, 15, 26, hwnd, (HMENU)EDIT3_ID, hIns, NULL);

    // ���b�Z�[�W���[�v
    while(GetMessage(&msg, NULL, 0, 0))
    {
        // ���b�Z�[�W���E�B���h�E�v���V�[�W���ɂ��n��
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

/*******************************
 * �����̍쐬
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
                ansNum.num1 = r; // 1�ڂɓ����
                cnt++;
                break;
            case 1:
                if(ansNum.num1 != r) // 1�ڂƈ�v���Ȃ�
                {
                    ansNum.num2 = r; // 2�ڂɓ����
                    cnt++;
                }
                break;
            case 2:
                if((ansNum.num1 != r) && (ansNum.num2 != r)) // 1�ځA2�ڂƈ�v���Ȃ�
                {
                    ansNum.num3 = r; // 3�ڂɓ����
                    cnt++;
                }
                break;
        }
    }

    return 0;
}

/*******************************
 * �����̔���
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
