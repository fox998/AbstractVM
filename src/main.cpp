#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <type_traits>
#include <assert.h>
#include <memory>
#include <iostream>
#include <fstream>

#include <regex>

#include "Operand.hpp"
#include "VirtualMachine.hpp"


#define ALL_RANGE( CONTAINER ) CONTAINER.begin(), CONTAINER.end()

using InstructionCallback = std::function<void(IOperand const*)>;

#define InstructionBuilderZeroParams(INST_NAME) \
{ #INST_NAME, [](auto param) {  assertion(param == nullptr, "excessive instruction param"); VirtualMachine::GetInstance().INST_NAME(); } }

#define InstructionBuilderOneParam(INST_NAME) \
{ #INST_NAME, [](auto param) { assertion(param != nullptr, "no instruction param"); VirtualMachine::GetInstance().instruction_##INST_NAME(param); } }

std::unordered_map<std::string_view, InstructionCallback> const instructions = {
    InstructionBuilderZeroParams(pop),
    InstructionBuilderZeroParams(dump),
    InstructionBuilderZeroParams(add),
    InstructionBuilderZeroParams(sub),
    InstructionBuilderZeroParams(mul),
    InstructionBuilderZeroParams(div),
    InstructionBuilderZeroParams(mod),
    InstructionBuilderZeroParams(print),
    InstructionBuilderZeroParams(exit),
    InstructionBuilderOneParam(push),
    InstructionBuilderOneParam(assert),
    {"", [](auto) {}}

};

std::unordered_map<std::string_view, eOperandType> const instructionParamToEnum = 
{
    { "int8", eOperandType::Int8 },
    { "int16", eOperandType::Int16 },
    { "int32", eOperandType::Int32 },
    { "float", eOperandType::Float },
    { "double", eOperandType::Double }
};

IOperand const* getInstructionParam(std::stringstream& ss)
{
    auto const[type, value] = [&ss]() -> std::pair<std::string, std::string>
    {
        if(ss.eof())
        {
            return {};
        }

        std::string param;
        ss >> param;

        if (param.size() == 0)
        {
            return {};
        }

        auto leftBracket = std::find( ALL_RANGE(param) , '(');
        auto rightBracket = std::find( ALL_RANGE(param) , ')');

        assertion(leftBracket != param.end() , "Invalid instruction param" );

        std::string const type (param.begin(), leftBracket);
        std::string const value (leftBracket + 1, rightBracket);

        return std::pair(type, value);
    }();


    if (type.size() == 0 && value.size() == 0)
    {
        return nullptr;
    }

    auto const operandTypeIt = instructionParamToEnum.find(type);
    assertion(operandTypeIt != instructionParamToEnum.end(), "bad operand type");

    return TypesFabric::GetInstance().createOperand(
        operandTypeIt->second,
        value
    );
}


void processInstruction(std::string const& line)
{
    auto const conmetBegin = std::find( ALL_RANGE(line) , ';');
    std::stringstream ss(std::string(line.begin(), conmetBegin));
    std::string instruction;
    ss >> instruction;

    auto const instrunctionCallback = instructions.find(instruction);

    assertion(instrunctionCallback != instructions.cend(), "Unexpected instruction");

    instrunctionCallback->second(
        getInstructionParam(ss)
    );

}


std::optional<std::fstream> getFstream(int argc, char **argv)
{
    if (argc > 1)
    {
        std::fstream stream(argv[1]);
        assertion(stream.is_open(), "Cant open file");
        return std::move( stream );
    }

    return {};
}


int main(int argc, char **argv)
{

    size_t lineNum = 0;
    try
    {
        std::optional<std::fstream> fstream = getFstream(argc, argv);
        auto& steam = ( fstream.has_value() ? fstream.value() : std::cin);
        
        for(std::string line; std::getline(steam, line); )
        {
            lineNum++;

            if (line == ";;")
            {
                processInstruction("exit");
            }

            processInstruction(line);
        }

        assertion(false, "No exit instruction");
    }
    catch(const std::exception& e)
    {
        std::cerr << "line " << lineNum << " : " << e.what() << '\n';
    }
}
