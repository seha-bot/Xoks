player = 0
board = 4
spacesRequiredToWin = 4
turn = board * board
place = ['-'] * turn

def index(x, y):
    if x >= 0 and x < board and y >= 0 and y < board:
        return x + y * board
    else:
        return -1

def printPlace():
    for y in range(board):
        for x in range(board):
            print(place[index(x, y)], end='')
        print("")

def changePlace(loc):
    if place[loc] == '-':
        if player == 0: place[loc] = 'x'
        if player == 1: place[loc] = 'o'
        return 0
    else:
        print('Cannot place there.')
        return 1

def check(x, y, ch, mode):
    win = 0
    for c in range(-spacesRequiredToWin + 1, spacesRequiredToWin - 1):
        i = 0
        if(mode == 'h'): i = index(x + c, y)
        if(mode == 'v'): i = index(x, y + c)
        if(mode == 'd1'): i = index(x + c, y + c)
        if(mode == 'd2'): i = index(x - c, y + c)
        if i != -1:
            if place[i] == ch:
                win += 1
    if win == spacesRequiredToWin: return 1
    else: return 0

def checkWin():
    for y in range(board):
        for x in range(board):
            winner = 'e'
            if place[index(x, y)] == 'x':
                winner = 'x'
            if place[index(x, y)] == 'o':
                winner = 'o'
            if winner != 'e':
                if check(x, y, winner, 'h') == 1 or check(x, y, winner, 'v') == 1 or check(x, y, winner, 'd1') == 1 or check(x, y, winner, 'd2') == 1:
                    print("Winner:", winner, "\n")
                    return 1

while 1:
    printPlace()
    if turn <= 0:
        break
    if checkWin() == 1:
        break
    choice = int(input("Pick your spot, 1-" + str(board * board) + ": "))
    if changePlace(choice - 1) == 0:
        turn -= 1
        player ^= 1