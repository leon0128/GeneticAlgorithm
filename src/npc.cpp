#include <iostream>
#include <deque>
#include <limits>
#include <utility>
#include <future>
#include <algorithm>
#include <thread>

#include "ga/evaluator.hpp"
#include "npc.hpp"

// 静的メンバ変数の初期化
NPC::VirtualGameState NPC::mVirtualGameState;
Actor::EType NPC::mActiveTetromino = NONE;
Actor::EType NPC::mHoldTetromino = NONE;
std::vector<Actor::EType> NPC::mNextTetromino;
bool NPC::mIsCalculating = false;
struct NPC::Result NPC::mResult;
std::vector<struct NPC::DetailResult> NPC::mDetailResultVector;
int NPC::mDenger = 0;

// ga
std::mutex NPC::mMutex;
std::mutex NPC::mTemporaryMutex;
std::mutex NPC::mScoreResultsMutex;
double mScoreResults;
NPC::Result NPC::mTemporaryResult = {0u, 0, false};

bool NPC::isCalculating()
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mIsCalculating;
}

const NPC::Result &NPC::getResult()
{
    std::lock_guard<std::mutex> lock(mMutex);
    return mResult;
}

void NPC::standCalculationFlag()
{
    std::lock_guard<std::mutex> lock(mMutex);
    mIsCalculating = true;
}

void NPC::startCalculation(EType active, 
                          EType hold, 
                          const std::vector<EType> &next,
                          const std::vector<std::array<Block*, GAMEBOARD_PARALLEL>> &gameState)
{
    // 結果の初期化
    mMutex.lock();
    mResult.isHoled = 0;
    mResult.direction = 0;
    mResult.coordinate = 0;
    mMutex.unlock();

    // 各メンバ変数の設定
    mActiveTetromino = active;
    mHoldTetromino = hold;
    mNextTetromino = next;

    // mVirtualGameStateの設定
    // for(int y = 0; y < (int)gameState.size(); y++)
    // {
    //     std::array<bool, GAMEBOARD_PARALLEL> line;
    //     mVirtualGameState.push_back(line);
    //     for(int x = 0; x < (int)gameState[y].size(); x++)
    //     {
    //         if(gameState[y][x])
    //         {
    //             mVirtualGameState[y][x] = true;
    //         }
    //         else
    //         {
    //             mVirtualGameState[y][x] = false;
    //         }
    //     }
    // }

    // ga
    for(auto &&arr : gameState)
    {
        mVirtualGameState.emplace_back();
        for(std::size_t i = 0; i < arr.size(); i++)
            mVirtualGameState.back()[i] = arr[i] != nullptr;
    }

    // 計算結果をmResultに格納
    calculateGA();
    // mVirticalGameStateの削除
    mVirtualGameState.clear();
    mDetailResultVector.clear();

    // 計算終了フラグ
    mTemporaryMutex.lock();
    mMutex.lock();
    mResult = mTemporaryResult;
    mIsCalculating = false;
    mMutex.unlock();
    mTemporaryMutex.unlock();
}

void NPC::calculate()
{
    // VirtualGameState gameState;

    // // ホールドと通常の合わせて80回
    // for(int h = 0; h < 2; h++)
    // {
    //     EType type = mActiveTetromino;
    //     if(h == 1)
    //     {
    //         if(mHoldTetromino == NONE)
    //         {
    //             type = mNextTetromino[0];
    //         }
    //         else
    //         {
    //             type = mHoldTetromino;
    //         }
    //     }

    //     for(unsigned int d = 0; d < 4; d++)
    //     {
    //         for(int c = 0; c < 10; c++)
    //         {
    //             // gameStateの作成
    //             gameState = updateGameState(mVirtualGameState,
    //                                         type,
    //                                         d,
    //                                         c);

    //             gameState = deleteLine(gameState);
    //             Result result = {d, c, h ? true : false};
    //             DetailResult detail = {result,
    //                                 getEmptyNumber(gameState),
    //                                 getMaxHeight(gameState),
    //                                 getDispersion(gameState),
    //                                 getHeightDifference(gameState)};
    //             mDetailResultVector.push_back(detail);
    //             gameState.clear(); 
    //         }
    //     }
    // }

    // if(getMaxHeight(mVirtualGameState) > 6 &&
    //    getMaxHeight(mVirtualGameState) <= 8 && 
    //    mDenger == 0)
    // {
    //     SDL_Log("=CAUTION=");
        
    //     deleteNonMinimumHeightDifference();
    //     deleteNonMinimumEmpty();
    //     deleteNonMinimumDispersion();
    //     mDenger += 2; 
    // }
    // else if(getMaxHeight(mVirtualGameState) > 8 &&
    //         getMaxHeight(mVirtualGameState) <= 10 &&
    //         mDenger == 0)
    // {
    //     SDL_Log("==WARNING==");
    //     deleteNonMinimumHeightDifference();
    //     deleteNonMinimumEmpty();
    //     deleteNonMinimumDispersion();
    //     mDenger += 4;         
    // }
    // else if(getMaxHeight(mVirtualGameState) > 10 &&
    //         getMaxHeight(mVirtualGameState) <= 14 &&
    //         mDenger == 0)
    // {
    //     SDL_Log("===DENGER===");
    //     deleteNonMinimumHeightDifference();
    //     deleteNonMinimumEmpty();
    //     deleteNonMinimumDispersion();
    //     mDenger += 6;         
    // }
    // else if(getMaxHeight(mVirtualGameState) > 14 &&
    //         mDenger == 0)
    // {
    //     SDL_Log("====DESPERATION====");
    //     deleteNonMinimumEmpty();
    //     deleteNonMinimumHeightDifference();
    //     deleteNonMinimumDispersion();
    //     mDenger += 2;
    // }
    // else
    // {
    //     deleteNonMinimumEmpty();
    //     deleteNonMinimumHeightDifference();
    //     deleteNonMinimumDispersion();
    //     if(mDenger > 0)
    //     {
    //         mDenger--;
    //     }
    // }

    // // 結果
    // if(!mDetailResultVector.empty())
    // {
    //     DetailResult detailResult = mDetailResultVector.at(rand() % mDetailResultVector.size());
    //     mResult = detailResult.result;
    //     // SDL_Log("emp: %d, dis: %lf, mHD: %d, mHe: %d", detailResult.empty, sqrt(detailResult.dispersion), detailResult.maxHeightDifference, detailResult.maxHeight);
    // }
    // else
    // {
    //     SDL_Log("NPC calculation result does not exist: %s", __func__);
    // }
}

void NPC::calculateGA()
{
    EType activeType = mActiveTetromino;
    EType holdType = mHoldTetromino;
    std::deque<EType> nextTypes(mNextTetromino.begin(), mNextTetromino.end());
    std::vector<int> deletedLines;

    if(0 < GA::Evaluator::NPC_DEPTH)
        supportCalculation(mVirtualGameState
            , activeType
            , holdType
            , nextTypes
            , deletedLines
            , 1);
}

double NPC::supportCalculation(const VirtualGameState &preState
    , EType activeType
    , EType holdType
    , const std::deque<EType> &inNextTypes
    , const std::vector<int> &inDeletedLines
    , int depth)
{
    static std::vector<std::pair<std::future<double>, Result>> futures;

    std::deque<EType> nextTypes(inNextTypes);
    std::vector<int> deletedLines(inDeletedLines);

    double ret = std::numeric_limits<double>::max();

    for(int h = 0; h < 2; h++)
    {
        if(h == 1)
        {
            if(holdType != NONE)
                std::swap(activeType, holdType);
            else if(!nextTypes.empty())
            {
                holdType = activeType;
                activeType = nextTypes.front();
                nextTypes.pop_front();
            }
            else
                break;
            
            if(activeType == holdType)
                break;
        }

        unsigned int dSize = 0u;
        switch(activeType)
        {
            case(EType::O):
                dSize = 1u;
            case(EType::I):
            case(EType::S):
            case(EType::Z):
                dSize = 2u;
            case(EType::J):
            case(EType::L):
            case(EType::T):
                dSize = 4u;
            
            default:;
        }
        for(unsigned int d = 0u; d < dSize; d++)
        {

            for(int c = 0; c < GAMEBOARD_PARALLEL; c++)
            {
                VirtualGameState state = preState; 
                updateGameState(state
                    , activeType
                    , d
                    , c);

                deletedLines.push_back(numDeletedLine(state));
                
                deleteLine(state);

                double score = std::numeric_limits<double>::max();
                if(depth == GA::Evaluator::NPC_DEPTH
                    || nextTypes.empty())
                {
                    score = GA::Evaluator::evaluate(getDispersion(state)
                        , getEmptyNumber(state)
                        , getMaxHeight(state)
                        , getHeightDifference(state)
                        , deletedLines);
                }
                else if(depth == 1)
                {
                    EType tmp = nextTypes.front();
                    nextTypes.pop_front();

                    futures.emplace_back(std::async(std::launch::async
                        , supportCalculation
                        , state
                        , tmp
                        , holdType
                        , nextTypes
                        , deletedLines
                        , depth + 1)
                        , Result{d, c, h ? true : false});
                
                    nextTypes.push_front(tmp);
                }
                else
                {
                    EType tmp = nextTypes.front();
                    nextTypes.pop_front();
                    
                    score = supportCalculation(state
                        , tmp
                        , holdType
                        , nextTypes
                        , deletedLines
                        , depth + 1);
                    nextTypes.push_front(tmp);
                }
                
                deletedLines.pop_back();
            
                if(score < ret)
                {
                    ret = score;
                    if(depth == 1)
                    {
                        mTemporaryMutex.lock();
                        mTemporaryResult = {d, c, h ? true : false};
                        mTemporaryMutex.unlock();
                    }
                }
            }
        }
    }

    if(depth == 1
        && !futures.empty())
    {
        std::size_t idx = 0;
        for(std::size_t i = 0; i < futures.size(); i++)
        {
            double score = futures[i].first.get();
            if(score < ret)
            {
                ret = score;
                idx = i;
            }
        }

        mTemporaryMutex.lock();
        mTemporaryResult = futures[idx].second;
        mTemporaryMutex.unlock();
        
        futures.clear();
    }

    return ret;
}

int NPC::getMinHeight(VirtualGameState gameState)
{
    // 各x軸で一番高い位置にあるブロックのy座標を格納
    // ブロックが存在しない場合は、-1となる
    std::array<int, GAMEBOARD_PARALLEL> maxHeights;
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        for(int y = GAMEBOARD_VERTICAL -1; y >= 0; y--)
        {
            if(gameState.at(y).at(x))
            {
                maxHeights.at(x) = y;
                break;
            }
            else if(y == 0)
            {
                maxHeights.at(x) = -1;
            }
        }
    }

    int minHeight = std::numeric_limits<int>::max();
    for(auto height : maxHeights)
    {
        if(height < minHeight)
        {
            minHeight = height;
        }
    }
    return minHeight;
}

int NPC::getMinHeight(VirtualGameState gameState, int exclusionX)
{
    // 各x軸で一番高い位置にあるブロックのy座標を格納
    // ブロックが存在しない場合は、-1となる
    std::array<int, GAMEBOARD_PARALLEL> maxHeights;
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        if(x != exclusionX)
        {
            for(int y = GAMEBOARD_VERTICAL -1; y >= 0; y--)
            {
                if(gameState.at(y).at(x))
                {
                    maxHeights.at(x) = y;
                    break;
                }
                else if(y == 0)
                {
                    maxHeights.at(x) = -1;
                }
            }
        }
        else
        {
            maxHeights.at(exclusionX) = std::numeric_limits<int>::max();
        }
    }

    int minHeight = std::numeric_limits<int>::max();
    for(auto height : maxHeights)
    {
        if(height < minHeight)
        {
            minHeight = height;
        }
    }
    return minHeight;
}

void NPC::updateGameState(VirtualGameState &gameState,
                         EType type,
                         int direction,
                         int coordinate)
{
    std::array<Vector2, 4> tetromino;
    getInitializeTetrominoCoordinate(type, tetromino);
    getRotationTetrominoCoordinate(tetromino, direction);
    getParallelTetrominoCoordinate(tetromino, coordinate);
    getQuickDropedGameState(gameState, tetromino);
}

void NPC::getInitializeTetrominoCoordinate(EType type
    , std::array<Vector2, 4> &dst)
{
    switch(type)
    {
        case(I):
            dst[0].set(4, 19);
            dst[1].set(5, 19);
            dst[2].set(3, 19);
            dst[3].set(6, 19);
            break;
        
        case(O):
            dst[0].set(4, 19);
            dst[1].set(4, 18);
            dst[2].set(5, 18);
            dst[3].set(5, 19);
            break;

        case(T):
            dst[0].set(4, 18);
            dst[1].set(4, 19);
            dst[2].set(3, 18);
            dst[3].set(5, 18);
            break;

        case(L):
            dst[0].set(4, 18);
            dst[1].set(5, 18);
            dst[2].set(3, 18);
            dst[3].set(5, 19);
            break;

        case(J):
            dst[0].set(4, 18);
            dst[1].set(3, 18);
            dst[2].set(5, 18);
            dst[3].set(3, 19);
            break;

        case(S):
            dst[0].set(4, 18);
            dst[1].set(4, 19);
            dst[2].set(3, 18);
            dst[3].set(5, 19);
            break;

        case(Z):
            dst[0].set(4, 18);
            dst[1].set(4, 19);
            dst[2].set(5, 18);
            dst[3].set(3, 19);
            break;
        
        default:
            SDL_Log("Cannot create tetromino: %s", __func__);
            break;
    }
}

void NPC::getRotationTetrominoCoordinate(std::array<Vector2, 4> &tetromino,
                                                          int direction)
{
    Vector2 target, distance;
    for(int i = 0; i < direction % 4; i++)
    {
        for(int j = 0; j < (int)tetromino.size(); j++)
        {
            target = tetromino[j];
            distance = target - tetromino[0];

            target.x += distance.y - distance.x;
            target.y += (int)(-1) * distance.x - distance.y;

            tetromino[j] = target;
        }
    }
}

void NPC::getParallelTetrominoCoordinate(std::array<Vector2, 4> &tetromino,
                                                      int coordinateX)
{
    bool isCorrect = true;
    while(tetromino[0].x != coordinateX && isCorrect)
    {
        
        if(coordinateX < tetromino[0].x)
        {
            for(int i = 0; i < (int)tetromino.size(); i++)
            {
                tetromino[i].x -= 1;
                if(tetromino[i].x < 0)
                {
                    isCorrect = false;
                }
            }
            if(!isCorrect)
            {
                for(int i = 0; i < (int)tetromino.size(); i++)
                {
                    tetromino[i].x += 1;
                }
            }
        }

        if(coordinateX > tetromino[0].x)
        {
            for(int i = 0; i < (int)tetromino.size(); i++)
            {
                tetromino[i].x += 1;
                if(tetromino[i].x > 9)
                {
                    isCorrect = false;
                }
            }
            if(!isCorrect)
            {
                for(int i = 0; i < (int)tetromino.size(); i++)
                {
                    tetromino[i].x -= 1;
                }
            }
        }
    }
}

void NPC::getQuickDropedGameState(VirtualGameState &gameState,
                                  std::array<Vector2, 4> &tetromino)
{
    bool isCorrect = true;
    while(isCorrect)
    {
        for(int i = 0; i < (int)tetromino.size(); i++)
        {
            tetromino[i].y -= 1;
            if(tetromino[i].y < 0 ||
               gameState[tetromino[i].y][tetromino[i].x])
            {
                isCorrect = false;
            }
        }
        if(!isCorrect)
        {
            for(int i = 0; i < (int)tetromino.size(); i++)
            {
                tetromino[i].y += 1;
            }
        }
    }

    // テトロミノの固定
    for(int i = 0; i < (int)tetromino.size(); i++)
    {
        gameState[tetromino[i].y][tetromino[i].x] = true;
    }
}

void NPC::deleteLine(VirtualGameState &gameState)
{
    std::vector<int> filledLine;
    for(int y = 0; y < GAMEBOARD_VERTICAL; y++)
    {
        for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
        {
            if(!gameState[y][x])
            {
                break;
            }
            else if(x == GAMEBOARD_PARALLEL - 1)
            {
                filledLine.push_back(y);
            }
        }
    }

    while(!filledLine.empty())
    {
        gameState.erase(gameState.begin() + *(filledLine.begin()));
        std::array<bool, GAMEBOARD_PARALLEL> line = {false};
        gameState.push_back(line);
        for(int i = 0; i < (int)filledLine.size(); i++)
        {
            filledLine[i] -= 1;
        }
        filledLine.erase(filledLine.begin());
    }
}

int NPC::numDeletedLine(const VirtualGameState &state)
{
    int ret = 0;

    for(auto &&arr : state)
    {
        if(std::all_of(arr.begin(), arr.end(), [&](bool b){return b;}))
            ret++;
    }

    return ret;
}

bool NPC::isFilledX(VirtualGameState gameState,
                   int x,
                   int leastHeight)
{
    for(int y = leastHeight + 1; y < (int)gameState.size(); y++)
    {
        if(gameState[y][x])
        {
            return true;
        }
    }
    return false;
}

int NPC::getMaxHeight(const VirtualGameState &gameState)
{
    bool isEmpty = false;
    int height = 0;
    for(int y = 0; y < (int)gameState.size(); y++)
    {
        isEmpty = true;
        for(int x = 0; x < (int)gameState[y].size(); x++)
        {
            if(gameState[y][x])
            {
                height = y;
                isEmpty = false;
            }
        }
        if(isEmpty)
        {
            break;
        }
    }

    return height;
}

int NPC::getEmptyNumber(const VirtualGameState &gameState)
{   
    // 各x軸で一番高い位置にあるブロックのy座標を格納
    // ブロックが存在しない場合は、-1となる
    std::array<int, GAMEBOARD_PARALLEL> maxHeights;
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        for(int y = GAMEBOARD_VERTICAL -1; y >= 0; y--)
        {
            if(gameState.at(y).at(x))
            {
                maxHeights.at(x) = y;
                break;
            }
            else if(y == 0)
            {
                maxHeights.at(x) = -1;
            }
        }
    }
    
    // 各x座標の最大値より下のブロックの空白の数を調査
    int emptyNumber = 0;
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        for(int y = 0; y < maxHeights.at(x); y++)
        {
            if(!gameState.at(y).at(x))
            {
                emptyNumber++;
            }
        }
    }
    
    return emptyNumber;
}

double NPC::getDispersion(const VirtualGameState &gameState)
{
    // 各x軸で一番高い位置にあるブロックのy座標を格納
    // ブロックが存在しない場合は、-1となる
    std::array<int, GAMEBOARD_PARALLEL> maxHeights;
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        for(int y = GAMEBOARD_VERTICAL -1; y >= 0; y--)
        {
            if(gameState.at(y).at(x))
            {
                maxHeights.at(x) = y + 1;
                break;
            }
            else if(y == 0)
            {
                maxHeights.at(x) = 0;
            }
        }
    }

    // 高さの分散を求める
    double dispersion = 0.0, average = 0.0, sum = 0;
    // 平均
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        sum += maxHeights.at(x);
    }
    average = sum / GAMEBOARD_PARALLEL;
    // 分散
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        dispersion = dispersion +
                     (maxHeights.at(x) - average) * 
                     (maxHeights.at(x) - average);
    }
    if(average != 0.0)
    {
        dispersion = dispersion / average;
    }
    else
    {
        dispersion = 0.0;
    }
    
    return dispersion;
}

int NPC::getHeightDifference(const VirtualGameState &gameState)
{
    // 各x軸で一番高い位置にあるブロックのy座標を格納
    // ブロックが存在しない場合は、0となる
    std::array<int, GAMEBOARD_PARALLEL> maxHeights;
    for(int x = 0; x < GAMEBOARD_PARALLEL; x++)
    {
        for(int y = GAMEBOARD_VERTICAL -1; y >= 0; y--)
        {
            if(gameState.at(y).at(x))
            {
                maxHeights.at(x) = y + 1;
                break;
            }
            else if(y == 0)
            {
                maxHeights.at(x) = 0;
            }
        }
    }

    // 隣接する列の高さの差を取得し、最も高いものを探す
    int maxDifference = -1;
    int diff = 0;
    for(int i = 0; i < (int)maxHeights.size() -1; i++)
    {
        diff = maxHeights.at(i) - maxHeights.at(i+1);
        if(diff < 0)
        {
            diff *= -1;
        }
        if(diff > maxDifference)
        {
            maxDifference = diff;
        }
    }
    
    return maxDifference;
}

void NPC::deleteNonMinimumEmpty()
{
    // emptyが最小のもの以外の削除
    int minEmpty = std::numeric_limits<int>::max();
    for(auto detail : mDetailResultVector)
    {
        if(detail.empty < minEmpty)
        {
            minEmpty = detail.empty;
        }
    }
    auto iterator = mDetailResultVector.begin();
    while(iterator != mDetailResultVector.end())
    {
        if(iterator->empty != minEmpty)
        {
            std::iter_swap(iterator, mDetailResultVector.end() -1);
            mDetailResultVector.pop_back();
        }
        else
        {
            iterator++;
        }
    }
}

void NPC::deleteNonMinimumHeight()
{
    // maxHeightが最小のもの
    int minHeight = std::numeric_limits<int>::max();
    for(auto detail : mDetailResultVector)
    {
        if(detail.maxHeight < minHeight)
        {
            minHeight = detail.maxHeight;
        }
    }
    auto iterator = mDetailResultVector.begin();
    while(iterator != mDetailResultVector.end())
    {
        if(iterator->maxHeight != minHeight)
        {
            std::iter_swap(iterator, mDetailResultVector.end() -1);
            mDetailResultVector.pop_back();
        }
        else
        {
            iterator++;
        }
    }
}

void NPC::deleteNonMinimumDispersion()
{
    // dispersionの最小値のみを残す
    double minDispersion = std::numeric_limits<double>::max();
    for(auto detail : mDetailResultVector)
    {
        if(detail.dispersion < minDispersion)
        {
            minDispersion = detail.dispersion;
        }
    }
    auto iterator = mDetailResultVector.begin();
    while(iterator != mDetailResultVector.end())
    {
        if(iterator->dispersion != minDispersion)
        {
            std::iter_swap(iterator, mDetailResultVector.end() -1);
            mDetailResultVector.pop_back();
        }
        else
        {
            iterator++;
        }
    }
}

void NPC::deleteNonMinimumHeightDifference()
{
    // maxDifferenceの最小値のみを残す
    int minHeightDifference = std::numeric_limits<int>::max();
    for(auto detail : mDetailResultVector)
    {
        if(detail.maxHeightDifference < minHeightDifference)
        {
            minHeightDifference = detail.maxHeightDifference;
        }
    }
    auto iterator = mDetailResultVector.begin();
    while(iterator != mDetailResultVector.end())
    {
        if(iterator->maxHeightDifference != minHeightDifference)
        {
            std::iter_swap(iterator, mDetailResultVector.end() -1);
            mDetailResultVector.pop_back();
        }
        else
        {
            iterator++;
        }
    }    
}

void NPC::printVirtualGameState(VirtualGameState gameState)
{
    for(int y = (int)gameState.size() - 1; y >= 0; y--)
    {
        for(int x = 0; x < (int)gameState[y].size(); x++)
        {
            if(gameState[y][x])
            {
                printf("||");
            }
            else
            {
                printf("--");
            }
        }
        printf(" :%d\n", y);
    }
    printf("0 1 2 3 4 5 6 7 8 9 \n");
}