import random  # 랜덤 모듈을 가져옵니다.

# 카드 덱을 초기화하는 함수입니다.
def initialize_deck():
    # 1부터 8까지의 숫자를 두 번씩 포함하는 리스트를 만듭니다.
    deck = list(range(1, 9)) * 2
    random.shuffle(deck)  # 덱을 무작위로 섞습니다.
    return deck  # 섞인 덱을 반환합니다.

# 게임 보드를 출력하는 함수입니다.
def print_board(board, revealed):
    for i in range(4):  # 4x4 보드를 출력합니다.
        for j in range(4):
            if revealed[i][j]:  # 카드가 뒤집혀져 있는 경우
                print(board[i][j], end=' ')  # 카드의 숫자를 출력합니다.
            else:  # 카드가 뒤집혀져 있지 않은 경우
                print('X', end=' ')  # 'X'를 출력합니다.
        print()  # 줄 바꿈을 합니다.

# 게임이 끝났는지 확인하는 함수입니다.
def check_game_over(revealed):
    for row in revealed:
        if not all(row):  # 모든 카드가 뒤집혀져 있는지 확인합니다.
            return False  # 하나라도 뒤집혀져 있지 않으면 False를 반환합니다.
    return True  # 모든 카드가 뒤집혀져 있으면 True를 반환합니다.

# 메인 게임 함수입니다.
def play_game():
    deck = initialize_deck()  # 덱을 초기화합니다.
    board = [deck[i*4:(i+1)*4] for i in range(4)]  # 4x4 보드로 변환합니다.
    revealed = [[False] * 4 for _ in range(4)]  # 모든 카드가 뒤집혀져 있지 않은 상태로 초기화합니다.

    while not check_game_over(revealed):  # 게임이 끝날 때까지 반복합니다.
        print_board(board, revealed)  # 현재 보드를 출력합니다.
        try:
            # 첫 번째 카드의 위치를 입력받습니다.
            x1, y1 = map(int, input("첫 번째 카드의 위치를 입력하세요 (예: 0 1): ").split())
            # 두 번째 카드의 위치를 입력받습니다.
            x2, y2 = map(int, input("두 번째 카드의 위치를 입력하세요 (예: 2 3): ").split())
        except ValueError:
            print("잘못된 입력입니다. 다시 시도하세요.")
            continue

        # 입력된 위치가 유효한지 확인합니다.
        if (x1 == x2 and y1 == y2) or not (0 <= x1 < 4 and 0 <= y1 < 4) or not (0 <= x2 < 4 and 0 <= y2 < 4):
            print("잘못된 위치입니다. 다시 시도하세요.")
            continue

        # 선택한 두 카드의 숫자가 같은지 확인합니다.
        if board[x1][y1] == board[x2][y2]:
            revealed[x1][y1] = True  # 첫 번째 카드를 뒤집습니다.
            revealed[x2][y2] = True  # 두 번째 카드를 뒤집습니다.
            print("맞췄습니다!")
        else:
            print("틀렸습니다. 다시 시도하세요.")

    print("축하합니다! 모든 카드를 맞췄습니다!")  # 게임 종료 메시지를 출력합니다.

# 게임을 시작합니다.
play_game()
