#ifndef MINI_MAX_WITH_PRUNING_H_
#define MINI_MAX_WITH_PRUNING_H_

#include <vector>
#include <utility>
#include <memory>
#include <iostream>
#include <unordered_map>

#include "Player.hpp"

struct State
{
  class NoEqualityAndHashImplementationException : public std::exception
  {
  public:
    virtual const char* what() const throw() override { return "No equality operator overloaded or hash provided in derived state - In order to use transposition table you need to specify equality between derived states and a hash of a derived state"; }
  };

  virtual ~State() = default;
  virtual std::size_t getHash() const { throw NoEqualityAndHashImplementationException(); return 0; }
  virtual bool operator==(const std::shared_ptr<State>& rhs) const { throw State::NoEqualityAndHashImplementationException(); return false; }
};

struct StateSharedPointerEquality 
{
  bool operator()(const std::shared_ptr<State>& lhs, const std::shared_ptr<State>& rhs) const
  {
    return *lhs == rhs;
  }
};

struct StateSharedPointerHash
{
  std::size_t operator()(const std::shared_ptr<State>& state) const
  {
    return state->getHash();
  }
};

struct Action
{
  virtual ~Action() = default;
};

struct ActionValue
{
  std::shared_ptr<Action> action;
  int value;
};

//TODO: put limit on size of map

class SearchableGame
{
public:
  virtual std::vector<std::pair<std::shared_ptr<State>, std::shared_ptr<Action>>> successorStates(const std::shared_ptr<State>& state) const = 0;
  virtual bool terminalState(const std::shared_ptr<State>& state) const = 0;
  virtual int getUtility(const std::shared_ptr<State>& state, const Player& player) const = 0;
  virtual Player getPlayerFromState(const std::shared_ptr<State>& state) const = 0;
  virtual std::shared_ptr<State> getState() const = 0;
  virtual void printState(const std::shared_ptr<State>& state) const { std::cout << "State print undefined" << std::endl; }
  virtual void printAction(const std::shared_ptr<Action>& action) const { std::cout << "Action print undefined" << std::endl; }

  virtual ~SearchableGame() = default;
};

class AlphaBetaSearch
{
public:
  enum class Options
  {
    USE_TRANSPOSITION_TABLE,
    USE_PRUNING
  };

  AlphaBetaSearch(const std::shared_ptr<SearchableGame>& game) : game(game), player(game->getPlayerFromState(game->getState())), depth(-1), transpositionTable() {}
  ActionValue performSearch();
  ActionValue performSearch(const std::vector<Options>& options);
  ActionValue performSearch(const std::vector<Options>& options, int depth);
  ActionValue performSearch(const std::shared_ptr<State>& state, const std::vector<Options>& options);
  ActionValue performSearch(const std::shared_ptr<State>& state, const std::vector<Options>& options, int depth);
  void test();

private:
  std::shared_ptr<const SearchableGame> game;
  Player player;
  int depth;
  std::unordered_map<std::shared_ptr<State>, ActionValue, StateSharedPointerHash, StateSharedPointerEquality> transpositionTable;
  ActionValue maxValue(const std::shared_ptr<State>& state);
  ActionValue minValue(const std::shared_ptr<State>& state);
  ActionValue minValueWithPruning(const std::shared_ptr<State>& state) { return ActionValue(); }
  ActionValue maxValueWithPruning(const std::shared_ptr<State>& state) { return ActionValue(); }
  ActionValue maxValueWithTranspositionTable(const std::shared_ptr<State>& state);
  ActionValue minValueWithTranspositionTable(const std::shared_ptr<State>& state);
  ActionValue minValueWithTranspositionTableAndPruning(const std::shared_ptr<State>& state) { return ActionValue(); }
  ActionValue maxValueWithTranspositionTableAndPruning(const std::shared_ptr<State>& state) { return ActionValue(); }
};

#endif