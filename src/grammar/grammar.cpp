#include "grammar.hpp"
#include <sstream>
/*
В первой строке содержатся 3 целых числа ∣N∣,∣Σ∣ и ∣P∣ --- количество нетерминальных символов, терминальных символов и правил в порождающей грамматике. Все числа неотрицательные и не превосходят 100.
Во второй строке содержатся ∣N∣ нетерминальных символов. Нетерминальные символы являются заглавными латинскими буквами.
В третьей строке содержатся ∣Σ∣ символов алфавита. Символы являются строчными латинскими буквами, цифрами, скобками или знаками арифметических операций.

В каждой из следующих P строк записано одно правило грамматики в формате левая часть правила -> правая части правила. 
ε в правой части правила обозначается отсутствием правой части (концом строки после ->).

Следующая строка состоит из одного нетерминального символа --- стартового символа грамматики.
*/
void Grammar::CreateFromStdin() {  
	std::cin >> neTerminalsSize_ >> alphabetSize_ >> rulesSize_;

	if (neTerminalsSize_ > 100) {
		throw GrammarException("Number of not terminals must me below 100 or 100.");
	} else if (alphabetSize_ > 100) {
		throw GrammarException("Number of terminals must me below 100 or 100.");
	} else if (rulesSize_ > 100) {
		throw GrammarException("Number of rules must me below 100 or 100.");
	} 

	for (size_t i = 0; i < neTerminalsSize_; ++i) {
		char neTerminal = 0;
		std::cin >> neTerminal;
		neTerminals_.push_back (NeTerminal(neTerminal));
	}

	for (size_t i = 0; i < alphabetSize_; ++i) {
		char terminal = 0;
		std::cin >> terminal;
		alphabet_.push_back (Terminal(terminal));
	}

	char skip;
	std::cin.get(skip);

	try {
		for (size_t i = 0; i < rulesSize_; ++i) {
			ReadRuleFromStdin();
		}
	} catch (GrammarException e) {
		throw e;
	}

	char start;	
	std::cin>>start;
	if(!IsNeTerminal(start)){
		throw GrammarException("Error: start must be non-terminal.");
	}
	start_ = NeTerminal(start);
}

void Grammar::ReadRuleFromStdin() {
	std::string rule;
	std::getline(std::cin, rule);

	std::istringstream ruleStream(rule);


	char left = 0;
	ruleStream >> left;

	if (!IsNeTerminal(left)) {
		throw GrammarException ("Wrong rule syntax: left part must consist from a non-terminal.");
	}	

	char next1 = 0;
	char next2 = 0;
	ruleStream >> next1;
	ruleStream >> next2;

	if (next1 != '-' || next2 != '>') {
		throw GrammarException ("Wrong rule syntax: rule doesn't contain '->' delimiter between left and right parts.");
	}


	char symbol = 0;
	std::vector<GrammarSymbol> right;
	while (ruleStream.peek() != EOF && ruleStream.peek() != '\n') {
		ruleStream >> symbol;
		if (IsNeTerminal(symbol)){
			right.push_back(NeTerminal(symbol));
		} else if (IsTerminal(symbol)) {
			right.push_back(Terminal(symbol));
		} else {
			throw GrammarException("Wrong rule syntax: right part contains unknown symbol.");
		}
	}

	if (!rules__.count(NeTerminal(left))){
		rules__[NeTerminal(left)] = std::vector<Rule>();
	}
	rules__[NeTerminal(left)].push_back(Rule(NeTerminal(left), right));
}

void Grammar::Print(){
	std::cout << "Alphabet:";
	for (auto elem: alphabet_) {
		std::cout << static_cast<char>(elem.symbol_) << ",";
	}
	std::cout << std::endl;

	std::cout << "Non-terminals:";
	for (auto elem: neTerminals_) {
		if (elem.symbol_ < 256) {
			std::cout << static_cast<char>(elem.symbol_) << ",";
		} else {
			std::cout << elem.symbol_ << ",";
		}
	}
	std::cout << std::endl;

	std::cout << "Rules:\n";
	for (auto& neTerminal: neTerminals_) {
		for (auto& rule: rules__[neTerminal]) {
			rule.Print();
		}
	}
	std::cout << std::endl;
}

void Grammar::ConvertToChomsky() {
    
}

void Grammar::RemoveLongRules() {
	for (auto elem: neTerminals_) {
		std::vector<Rule> new_rules;
		for (auto it = rules__[elem].begin(), end = rules__[elem].end(); it != end; ++it) {
			if (it->right_.size() > 2) {
				RemoveLongRule(new_rules, *it);
			}
		}
	}
}

void Grammar::RemoveLongRule(std::vector<Rule>& new_rules, Rule& rule) {

}
