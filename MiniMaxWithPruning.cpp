#include "MiniMaxWithPruning.hpp"

#include <limits.h>
#include <exception>
#include <iostream>
#include <algorithm>

int count = 0;

void AlphaBetaSearch::test()
{
  //transpositionTable[game->getState()] = 1;
  //std::cout << transpositionTable.at(game->getState()) << std::endl;
}

ActionValue AlphaBetaSearch::performSearch()
{ 
  std::vector<Options> options;
  return performSearch(game->getState(), options, -1);
}

ActionValue AlphaBetaSearch::performSearch(const std::vector<Options>& options)
{
  return performSearch(game->getState(), options, -1);
}

ActionValue AlphaBetaSearch::performSearch(const std::shared_ptr<State>& state, const std::vector<Options>& options, int depth)
{
  player = game->getPlayerFromState(state);
  this->depth = depth;

  if (std::find(options.begin(), options.end(), Options::USE_TRANSPOSITION_TABLE) != options.end())
    return maxValueWithTranspositionTable(state);

  return maxValue(state);
}

ActionValue AlphaBetaSearch::maxValue(const std::shared_ptr<State>& state)
{
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MIN;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;

    ActionValue newActionValue = minValue(state);
    newActionValue.action = successor.second;

    if (newActionValue.value > actionValue.value)
      actionValue = newActionValue;
  }

  return actionValue;
}

ActionValue AlphaBetaSearch::minValue(const std::shared_ptr<State>& state)
{
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MAX;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;
    ActionValue newActionValue = maxValue(state);
    newActionValue.action = successor.second;

    if (newActionValue.value < actionValue.value)
      actionValue = newActionValue;
  }

  return actionValue;
}

ActionValue AlphaBetaSearch::maxValueWithTranspositionTable(const std::shared_ptr<State>& state)
{
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MIN;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;
    ActionValue newActionValue;
    if (transpositionTable.find(state) != transpositionTable.end())
      newActionValue = transpositionTable.at(state);
    else
    {
      newActionValue = minValueWithTranspositionTable(state);
      transpositionTable[state] = newActionValue;
    }
    
    newActionValue.action = successor.second;

    if (newActionValue.value > actionValue.value)
      actionValue = newActionValue;
  }

  return actionValue;
}

ActionValue AlphaBetaSearch::minValueWithTranspositionTable(const std::shared_ptr<State>& state)
{
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MAX;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;
    ActionValue newActionValue;
    if (transpositionTable.find(state) != transpositionTable.end())
      newActionValue = transpositionTable.at(state);
    else
    {
      newActionValue = maxValueWithTranspositionTable(state);
      transpositionTable[state] = newActionValue;
    }

    newActionValue.action = successor.second;

    if (newActionValue.value < actionValue.value)
      actionValue = newActionValue;
  }

  return actionValue;
}