#include "MiniMax.hpp"

#include <limits.h>
#include <exception>
#include <iostream>
#include <algorithm>

ActionValue MiniMaxSearch::performSearch()
{ 
  std::vector<Options> options;
  return performSearch(game->getState(), options, -1);
}

ActionValue MiniMaxSearch::performSearch(const std::shared_ptr<State>& state)
{ 
  std::vector<Options> options;
  return performSearch(state, options, -1);
}

ActionValue MiniMaxSearch::performSearch(const std::shared_ptr<State>& state, int depth)
{
  std::vector<Options> options;
  return performSearch(state, options, depth);
}

ActionValue MiniMaxSearch::performSearch(const std::shared_ptr<State>& state, const std::vector<Options>& options)
{
  return performSearch(state, options, -1);
}

ActionValue MiniMaxSearch::performSearch(const std::shared_ptr<State>& state, const std::vector<Options>& options, int depth)
{
  player = game->getPlayerFromState(state);
  depthLimit = depth;
  int currentDepth = 0;
  ActionValue actionValue;
  transpositionTable.clear();

  if (std::find(options.begin(), options.end(), Options::USE_TRANSPOSITION_TABLE) != options.end())
  {
    if (std::find(options.begin(), options.end(), Options::USE_PRUNING) != options.end())
      actionValue = maxValueWithTranspositionTableAndPruning(state, INT_MIN, INT_MAX, currentDepth);
    else
      actionValue = maxValueWithTranspositionTable(state, currentDepth);
  }
  else
  {
    if (std::find(options.begin(), options.end(), Options::USE_PRUNING) != options.end())
      actionValue = maxValueWithPruning(state, INT_MIN, INT_MAX, currentDepth);
    else
      actionValue = maxValue(state, currentDepth);
  }
  
  depthLimit = -1;
  return actionValue;
}

ActionValue MiniMaxSearch::performSearch(const std::vector<Options>& options)
{
  return performSearch(game->getState(), options, -1);
}

ActionValue MiniMaxSearch::performSearch(const std::vector<Options>& options, int depth)
{
  return performSearch(game->getState(), options, depth);
}

ActionValue MiniMaxSearch::performSearch(int depth)
{
  std::vector<Options> options;
  return performSearch(game->getState(), options, depth);
}

ActionValue MiniMaxSearch::maxValue(const std::shared_ptr<State>& state, int currentDepth)
{
  currentDepth++;
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};

  if (depthLimit != -1)
    if (currentDepth >= depthLimit)
      return {nullptr, game->getEvaluationValue(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MIN;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;

    ActionValue newActionValue = minValue(state, currentDepth);
    newActionValue.action = successor.second;

    if (newActionValue.value > actionValue.value)
      actionValue = newActionValue;
  }

  return actionValue;
}

ActionValue MiniMaxSearch::minValue(const std::shared_ptr<State>& state, int currentDepth)
{
  currentDepth++;
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};

  if (depthLimit != -1)
    if (currentDepth >= depthLimit)
      return {nullptr, game->getEvaluationValue(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MAX;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;
    ActionValue newActionValue = maxValue(state, currentDepth);
    newActionValue.action = successor.second;

    if (newActionValue.value < actionValue.value)
      actionValue = newActionValue;
  }

  return actionValue;
}

ActionValue MiniMaxSearch::maxValueWithPruning(const std::shared_ptr<State>& state, int alpha, int beta, int currentDepth)
{
  currentDepth++;
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};

  if (depthLimit != -1)
    if (currentDepth >= depthLimit)
      return {nullptr, game->getEvaluationValue(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MIN;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;

    ActionValue newActionValue = minValueWithPruning(state, alpha, beta, currentDepth);
    newActionValue.action = successor.second;

    if (newActionValue.value > actionValue.value)
      actionValue = newActionValue;

    if (actionValue.value >= beta)
      return actionValue;

    alpha = std::max(alpha, actionValue.value);
  }

  return actionValue;
}

ActionValue MiniMaxSearch::minValueWithPruning(const std::shared_ptr<State>& state, int alpha, int beta, int currentDepth)
{
  currentDepth++;
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};

  if (depthLimit != -1)
    if (currentDepth >= depthLimit)
      return {nullptr, game->getEvaluationValue(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MAX;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;
    ActionValue newActionValue = maxValueWithPruning(state, alpha, beta, currentDepth);
    newActionValue.action = successor.second;

    if (newActionValue.value < actionValue.value)
      actionValue = newActionValue;
    
    if (actionValue.value <= alpha)
      return actionValue;

    beta = std::max(beta, actionValue.value);
  }

  return actionValue;
}

ActionValue MiniMaxSearch::maxValueWithTranspositionTable(const std::shared_ptr<State>& state, int currentDepth)
{
  currentDepth++;
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};
  
  if (depthLimit != -1)
    if (currentDepth >= depthLimit)
      return {nullptr, game->getEvaluationValue(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MIN;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;
    ActionValue newActionValue;
    if (transpositionTable.find(state) != transpositionTable.end())
      newActionValue.value = transpositionTable.at(state);
    else
    {
      newActionValue = minValueWithTranspositionTable(state, currentDepth);
      transpositionTable[state] = newActionValue.value;
    }
    
    newActionValue.action = successor.second;

    if (newActionValue.value > actionValue.value)
      actionValue = newActionValue;
  }

  return actionValue;
}

ActionValue MiniMaxSearch::minValueWithTranspositionTable(const std::shared_ptr<State>& state, int currentDepth)
{
  currentDepth++;
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};

  if (depthLimit != -1)
    if (currentDepth >= depthLimit)
      return {nullptr, game->getEvaluationValue(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MAX;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;
    ActionValue newActionValue;
    if (transpositionTable.find(state) != transpositionTable.end())
      newActionValue.value = transpositionTable.at(state);
    else
    {
      newActionValue = maxValueWithTranspositionTable(state, currentDepth);
      transpositionTable[state] = newActionValue.value;
    }

    newActionValue.action = successor.second;

    if (newActionValue.value < actionValue.value)
      actionValue = newActionValue;
  }

  return actionValue;
}

ActionValue MiniMaxSearch::maxValueWithTranspositionTableAndPruning(const std::shared_ptr<State>& state, int alpha, int beta, int currentDepth)
{
  currentDepth++;
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};
  
  if (depthLimit != -1)
    if (currentDepth >= depthLimit)
      return {nullptr, game->getEvaluationValue(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MIN;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;
    ActionValue newActionValue;
    if (transpositionTable.find(state) != transpositionTable.end())
      newActionValue.value = transpositionTable.at(state);
    else
    {
      newActionValue = minValueWithTranspositionTableAndPruning(state, alpha, beta, currentDepth);
      transpositionTable[state] = newActionValue.value;
    }
    
    newActionValue.action = successor.second;

    if (newActionValue.value > actionValue.value)
      actionValue = newActionValue;

    if (actionValue.value >= beta)
      return actionValue;
  }

  return actionValue;
}

ActionValue MiniMaxSearch::minValueWithTranspositionTableAndPruning(const std::shared_ptr<State>& state, int alpha, int beta, int currentDepth)
{
  currentDepth++;
  if (game->terminalState(state))
    return {nullptr, game->getUtility(state, player)};

  if (depthLimit != -1)
    if (currentDepth >= depthLimit)
      return {nullptr, game->getEvaluationValue(state, player)};
  
  ActionValue actionValue;
  actionValue.action = nullptr;
  actionValue.value = INT_MAX;

  for (auto successor : game->successorStates(state))
  {
    std::shared_ptr<State> state = successor.first;
    ActionValue newActionValue;
    if (transpositionTable.find(state) != transpositionTable.end())
      newActionValue.value = transpositionTable.at(state);
    else
    {
      newActionValue = maxValueWithTranspositionTableAndPruning(state, alpha, beta, currentDepth);
      transpositionTable[state] = newActionValue.value;
    }

    newActionValue.action = successor.second;

    if (newActionValue.value < actionValue.value)
      actionValue = newActionValue;

    if (actionValue.value <= alpha)
      return actionValue;

    beta = std::max(beta, actionValue.value);
  }

  return actionValue;
}
