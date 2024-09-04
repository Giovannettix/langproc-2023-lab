#include "ast.hpp"

#include <string>
#include <regex>

static int makeNameUnq=0;

static std::string makeName(std::string base)
{
    return "_"+base+"_"+std::to_string(makeNameUnq++);
}

void CompileRec(
    std::string destReg,    // The name of the register to put the result in
    TreePtr program
){
    std::regex reNum("^-?[0-9]+$");
    std::regex reId("^[a-z][a-z0-9]*$");
    
    if( regex_match( program->type, reNum ) ){
        std::cout<<"const "<<destReg<<" "<<program->type<<std::endl;
        
    }else if( regex_match( program->type, reId ) ){
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->type<<" "<<zero<<std::endl;

    }else if(program->type=="Input"){
        std::cout<<"input "<<destReg<<std::endl;

    }else if(program->type=="Param"){
        std::cout<<"param "<<destReg<<" "<<program->value<<std::endl;

    }else if(program->type=="Output"){
        CompileRec(destReg, program->branches.at(0));
        std::cout<<"output "<<destReg<<std::endl;

    }else if(program->type=="Seq"){
        for(unsigned i=0; i<program->branches.size(); i++){
            CompileRec(destReg, program->branches[i]);
        }
    
    // TODO : handle the others
    }else if(program->type=="Add"){
        std::string valA = makeName("valA");
        std::string valB = makeName("valB");
        CompileRec(valA, program->branches[0]);
        CompileRec(valB, program->branches[1]);
        std::cout<<"add "<<destReg<<" "<<valA<<" "<<valB<<std::endl;

    }else if(program->type=="Sub"){
        std::string valA = makeName("valA");
        std::string valB = makeName("valB");
        CompileRec(valA, program->branches[0]);
        CompileRec(valB, program->branches[1]);
        std::cout<<"sub "<<destReg<<" "<<valA<<" "<<valB<<std::endl;
        
    }else if(program->type=="LessThan"){
        std::string valA = makeName("valA");
        std::string valB = makeName("valB");
        CompileRec(valA, program->branches[0]);
        CompileRec(valB, program->branches[1]);
        std::cout<<"lt "<<destReg<<" "<<valA<<" "<<valB<<std::endl;

    }else if(program->type=="Assign"){
        CompileRec(program->value, program->branches[0]);
        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;
        std::cout<<"add "<<destReg<<" "<<program->value<<" "<<zero<<std::endl;

    }else if(program->type=="If"){

        std::string falseLbl = makeName("falseLbl");
        std::string end = makeName("endLbl");

        std::string cond = makeName("cond");
        CompileRec(cond, program->branches[0]);

        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;

        std::cout<<"beq "<<cond<<" "<<zero<<" "<<falseLbl<<std::endl;
        CompileRec(destReg, program->branches[1]);
        std::cout<<"beq "<<zero<<" "<<zero<<" "<<end<<std::endl;
        std::cout<<":"<<falseLbl<<std::endl;
        CompileRec(destReg, program->branches[2]);
        
        std::cout<<":"<<end<<std::endl;
    
    }else if(program->type =="While"){

        std::string loop = makeName("loop");
        std::string end = makeName("endLbl");
        std::string whileReg = makeName("whileReg");

        std::string zero = makeName("zero");
        std::cout<<"const "<<zero<<" 0"<<std::endl;

        std::cout<<":"<<loop<<std::endl;
        CompileRec(destReg, program->branches[0]);
        
        std::cout <<"beq "<<destReg<<" "<<zero<<" "<<end<<std::endl;
        CompileRec(whileReg, program->branches[1]);

        std::cout<<"beq "<<zero<<" "<<zero<<" "<<loop<<std::endl;
        std::cout<<":"<<end<<std::endl;

    }else{
        throw std::runtime_error("Unknown construct '"+program->type+"'");
    }
}

void Compile(
    TreePtr program
){
    // Create a register to hold the final result
    std::string res=makeName("res");
    
    // Compile the whole thing
    CompileRec(res, program);
    
    // Put the result out
    std::cout<<"halt "<<res<<"\n";
}
