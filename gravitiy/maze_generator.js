const GRID_SIZE = 30;
const CELL_SIZE = 20;

function generateMaze(width, height) {
    // 미로를 모두 벽으로 초기화
    const maze = Array(height).fill().map(() => Array(width).fill(1));
    const stack = [];
    const start = {x: 1, y: height - 2};
    const end = {x: width - 2, y: 1};
    
    function carve(x, y) {
        maze[y][x] = 0;
        const directions = [{x: 1, y: 0}, {x: -1, y: 0}, {x: 0, y: 1}, {x: 0, y: -1}];
        directions.sort(() => Math.random() - 0.5);
        
        for (let dir of directions) {
            const nx = x + dir.x * 2, ny = y + dir.y * 2;
            if (nx > 0 && ny > 0 && nx < width - 1 && ny < height - 1 && maze[ny][nx] === 1) {
                maze[y + dir.y][x + dir.x] = 0;
                stack.push({x: nx, y: ny});
            }
        }
    }
    
    carve(start.x, start.y);
    while (stack.length > 0) {
        const {x, y} = stack.pop();
        carve(x, y);
    }
    
    // 시작점과 끝점 설정
    maze[start.y][start.x] = 2;
    maze[end.y][end.x] = 3;
    
    // 외벽 생성
    for (let y = 0; y < height; y++) {
        maze[y][0] = 1;
        maze[y][width-1] = 1;
    }
    for (let x = 0; x < width; x++) {
        maze[0][x] = 1;
        maze[height-1][x] = 1;
    }
    
    return maze;
}

function createStages(numStages) {
    const stages = [];
    for (let i = 0; i < numStages; i++) {
        const maze = generateMaze(GRID_SIZE, GRID_SIZE);
        stages.push(maze);
    }
    return stages;
}
