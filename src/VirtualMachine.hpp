#pragma once


#include <vector>
#include <memory>
#include <functional>

#include "IOperand.hpp"


class VirtualMachine
{
public:

    static VirtualMachine& GetInstance() { static VirtualMachine vm; return vm; };

    // commpile error witout "instruction_" prefix
    void instruction_push(IOperand const* instructionOperand);
    void instruction_assert(IOperand const* instructionOperand);

    void pop();
    void dump();
    void add();
    void sub();
    void mul();
    void div();
    void mod();
    void print();

    void exit();

    ~VirtualMachine() = default;

private:
    VirtualMachine() = default;
    VirtualMachine(VirtualMachine&&) = delete;
    VirtualMachine(VirtualMachine const&) = delete;
    VirtualMachine& operator=(VirtualMachine&&) = delete;
    VirtualMachine& operator=(VirtualMachine const&) = delete;

    using OperandT = IOperand const*;
    void doubleOperatorWrapper(std::function<OperandT(OperandT, OperandT)>&& operation);

    std::vector<std::unique_ptr<IOperand const>> m_stack;
};

