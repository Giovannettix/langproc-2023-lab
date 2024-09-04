#include "ast.hpp"

#include <regex>

int32_t Interpret(
    InterpretContext &context, // Contains the parameters and variable bindings
    TreePtr program
){

    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match(program->type, reNum) ){
        return std::atol(program->type.c_str());

    }else if( regex_match(program->type, reId) ){
        return context.bindings[program->type]; 

    }else if(program->type == "Input"){
        std::string val;
        std::cin>>val;
        return std::stoi(val);

    }else if(program->type=="Param"){
        unsigned index=atol(program->value.c_str());
        auto value=context.params.at(index);
        return value;
        
    }else if(program->type=="Output"){
        int32_t val=Interpret(context, program->branches.at(0));
        std::cout<<val<<std::endl;
        return val;
        
        
    }else if(program->type == "LessThan"){
        int32_t a = Interpret(context, program->branches.at(0));
        int32_t b = Interpret(context, program->branches.at(1));
        return a < b;

    }else if(program->type == "Add"){
        int32_t a = Interpret(context, program->branches.at(0));
        int32_t b = Interpret(context, program->branches.at(1));
        return a + b;

    }else if(program->type == "Sub"){
        int32_t a = Interpret(context, program->branches.at(0));
        int32_t b = Interpret(context, program->branches.at(1));
        return a - b;

    }else if(program->type == "Assign"){
        int32_t val = Interpret(context, program->branches.at(0));
        context.bindings[program->value] = val;
        return val;

    }else if(program->type == "Seq"){
        int32_t val;
        for(int i=0; i<program->branches.size(); i++){
            val = Interpret(context, program->branches.at(i));
        }
        return val;

    }else if(program->type == "If"){
        int32_t val=Interpret(context, program->branches.at(0));
        if (val != 0){
            return Interpret(context, program->branches.at(1));
        }
        else{
            return Interpret(context, program->branches.at(2));
        }

    }else if(program->type == "While"){
        while(Interpret(context, program->branches.at(0)) != 0){
            Interpret(context, program->branches.at(1));
        }
        return 0;

    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}
