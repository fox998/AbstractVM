#include "VirtualMachine.hpp"

#include <iostream>

#include "Exception.hpp"
#include "Operand.hpp"

#define DOUBLE_OPERATOR( OPERATOR ) \
[](auto first, auto second){ return *second OPERATOR *first; }

void VirtualMachine::pop()
{
    m_stack.pop_back();
}


void VirtualMachine::dump()
{
    for(auto it = m_stack.rbegin(); it != m_stack.rend(); it++)
    {
        std::cout << (*it)->toString() << std::endl;
    }
}


void VirtualMachine::doubleOperatorWrapper(std::function<OperandT(OperandT, OperandT)>&& op)
{
    assertion(m_stack.size() > 1, "stack size less than 2");

    auto first = m_stack.rbegin();
    auto second = first + 1;

    auto result = op(first->get(), second->get());

    m_stack.erase(m_stack.end() - 2, m_stack.end());
    m_stack.emplace_back(result);
}

void VirtualMachine::add()
{
    doubleOperatorWrapper(DOUBLE_OPERATOR(+));
}

void VirtualMachine::sub()
{
    doubleOperatorWrapper(DOUBLE_OPERATOR(-));
}

void VirtualMachine::mul()
{
    doubleOperatorWrapper(DOUBLE_OPERATOR(*));
}

void VirtualMachine::div()
{
    doubleOperatorWrapper(DOUBLE_OPERATOR(/));
}

void VirtualMachine::mod()
{
    doubleOperatorWrapper(DOUBLE_OPERATOR(%));
}

void VirtualMachine::print()
{
    auto const& top = m_stack.back();
    assertion(top->getType() == eOperandType::Int8, "Not printable value");

    auto operand = static_cast<Operand<Int8> const*>(top.get());
    std::cout << static_cast<char>(operand->getValue()) << std::endl;

}

void VirtualMachine::exit()
{
    std::exit(EXIT_SUCCESS);
}


void VirtualMachine::instruction_push(IOperand const* operand)
{
    m_stack.emplace_back(operand);
}

void VirtualMachine::instruction_assert(IOperand const* instructionOperand)
{

    std::unique_ptr<IOperand const> result (*m_stack.back().get() - *instructionOperand);

    assertion(
        result->toString()[0] == '0',
        "assert false"
    );
    delete instructionOperand;
}
