let startButton = document.getElementById('start-button');
let gameBoard = document.getElementById('game-board');
let timerElement = document.getElementById('timer');
let timer;
let seconds = 0;
let firstCard = null;
let secondCard = null;
let lockBoard = false;
let matchedCards = 0;

startButton.addEventListener('click', startGame);

function startGame() {
    startButton.disabled = true;
    resetGame();
    createCards();
    // 모든 카드를 앞면으로 뒤집음
    let cards = document.querySelectorAll('.card');
    cards.forEach(card => card.classList.add('flipped'));

    // 1초 후에 카드를 다시 뒷면으로 돌리고 타이머 시작
    setTimeout(() => {
        cards.forEach(card => card.classList.remove('flipped'));
        lockBoard = false; // 플레이어가 카드를 클릭할 수 있도록 보드 잠금 해제
        timer = setInterval(() => {
            seconds++;
            timerElement.textContent = `시간: ${seconds}초`;
        }, 1000);
    }, 1000); // 1초 동안 카드 앞면 보여주기

    lockBoard = true; // 보드 잠금 설정
}

function createCards() {
    const symbols = ['♠', '♥', '♦', '♣', 'A', 'K', 'Q', 'J'];
    let cardArray = symbols.concat(symbols); // 같은 심볼 두 개씩
    cardArray.sort(() => 0.5 - Math.random()); // 카드 섞기

    cardArray.forEach(symbol => {
        let card = document.createElement('div');
        card.classList.add('card');

        let cardInner = document.createElement('div');
        cardInner.classList.add('card-inner');

        let cardFront = document.createElement('div');
        cardFront.classList.add('card-front');
        cardFront.textContent = symbol;

        let cardBack = document.createElement('div');
        cardBack.classList.add('card-back');

        cardInner.appendChild(cardFront);
        cardInner.appendChild(cardBack);
        card.appendChild(cardInner);

        card.dataset.symbol = symbol;

        card.addEventListener('click', flipCard);

        gameBoard.appendChild(card);
    });
}

function flipCard() {
    if (lockBoard) return;
    if (this === firstCard) return;

    this.classList.add('flipped');

    if (!firstCard) {
        firstCard = this;
        return;
    }

    secondCard = this;
    checkForMatch();
}

function checkForMatch() {
    let isMatch = firstCard.dataset.symbol === secondCard.dataset.symbol;

    if (isMatch) {
        disableCards();
        matchedCards += 2;
        if (matchedCards === 16) {
            clearInterval(timer);
            setTimeout(() => {
                alert(`게임 종료! 걸린 시간: ${seconds}초`);
                startButton.disabled = false;
            }, 500);
        }
    } else {
        unflipCards();
    }
}

function disableCards() {
    firstCard.removeEventListener('click', flipCard);
    secondCard.removeEventListener('click', flipCard);

    resetBoard();
}

function unflipCards() {
    lockBoard = true;
    setTimeout(() => {
        firstCard.classList.remove('flipped');
        secondCard.classList.remove('flipped');

        resetBoard();
    }, 1000);
}

function resetBoard() {
    [firstCard, secondCard, lockBoard] = [null, null, false];
}

function resetGame() {
    gameBoard.innerHTML = '';
    timerElement.textContent = '시간: 0초';
    clearInterval(timer);
    seconds = 0;
    matchedCards = 0;
    resetBoard();
}