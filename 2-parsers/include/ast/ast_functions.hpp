#ifndef ast_functions_hpp
#define ast_functions_hpp

#include "ast_expression.hpp"

#include <cmath>

// virtual = can be overriden in derived class
// override = function is intended to override virtual function in base class (Expression)
class Function
    : public Expression
{
private:
    ExpressionPtr arg;
protected: 

//constructor for Function class, which takes an ExpressionPtr _arg as an argument
//and intitialises arg in the function class to _arg

    Function(ExpressionPtr _arg)
        : arg(_arg)
    {} 
public:

    //destructor for Function class, only has to delete arg
    virtual ~Function()
    {
        delete arg;
    }

    //pure virtual function that does not have implementation in base class
    virtual const char * getFunction() const =0;

    ExpressionPtr getArg() const
    { return arg; }

    virtual void print(std::ostream &dst) const override
    {
        dst<<getFunction()<<"( ";   //prints out name of function (
        arg->print(dst);            //calls print method on arg
        dst<<" )";                  // )
    }

    virtual double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        // NOTE : This should be implemented by the inheriting function nodes, e.g. LogFunction
        throw std::runtime_error("FunctionOperator::evaluate is not implemented.");
    }
};

class LogFunction
    : public Function
{
public:
    LogFunction(ExpressionPtr _arg)
        : Function(_arg)
    {}

    virtual const char *getFunction() const
    { return "log"; }
    
    // TODO-E : Override evaluate, and implement it

    double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        double argValue = getArg()->evaluate(bindings);
        return std::log(argValue);
    }

};

class ExpFunction
    : public Function
{
public:
    ExpFunction(ExpressionPtr _arg)
        : Function(_arg)
    {}

    virtual const char *getFunction() const
    { return "exp"; }

    double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        double argValue = getArg()->evaluate(bindings);
        return std::exp(argValue);
    }

};

class SqrtFunction
    : public Function
{
public:
    SqrtFunction(ExpressionPtr _arg)
        : Function(_arg)
    {}

    virtual const char *getFunction() const
    { return "sqrt"; }

    double evaluate(
        const std::map<std::string,double> &bindings
    ) const override
    {
        double argValue = getArg()->evaluate(bindings);
        return std::sqrt(argValue);
    }

};


#endif
