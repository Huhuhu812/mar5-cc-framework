#include "IR.h"

#ifdef IR_DEBUG
#define DEBUG_OUT (std::cerr << "DEBUG-IR: ") // or any other ostream
#else
#define DEBUG_OUT 0 && std::cerr
#endif

namespace IR {

    ItemTypeSig::ItemTypeSig(VarType vtype) {
        this->itemtype = ItemType::item_type_sig;
        this->vtype = vtype;
        this->ndim = 0;
    }
    
    ItemTypeSig::ItemTypeSig(VarType vtype, int32_t ndim) {
        this->itemtype = ItemType::item_type_sig;
        this->vtype = vtype;
        this->ndim = ndim;
    }

    ItemConstant::ItemConstant(int64_t constVal) {
        this->itemtype = item_constant;
        this->constVal = constVal;
    }

    ItemLabel::ItemLabel(std::string str) {
        this->itemtype = item_labels;
        this->labelName = str;
    }

    ItemVariable::ItemVariable(std::string str, Item * type_sig){
        this->itemtype = item_variable;
        this->name = str;
        this->typeSig = type_sig;
    }

    ItemArrAccess::ItemArrAccess(Item * addr, std::vector<Item *> & offsets) {
        this->itemtype = item_ArrAccess;
        this->addr = addr;
        this->offsets = offsets;
    }

    ItemOp::ItemOp(Item *op1, Item *op2, OpType opType) {
        this->itemtype = item_op;
        this->op1 = op1;
        this->op2 = op2;
        this->opType = opType;
    }

    ItemCall::ItemCall(bool isRuntime, Item *callee, std::vector<Item *> & args) {
        this->itemtype = item_call;
        this->isRuntime = isRuntime;
        this->callee = callee;
        this->args = args;
    }

    ItemNewArray::ItemNewArray(std::vector<Item *> &dims) {
        this->itemtype = ItemType::item_newArr;
        this->dims = dims;    
    }

    ItemNewTuple::ItemNewTuple(Item *len) {
        this->itemtype = ItemType::item_newTuple;
        this->len = len;
    }
    
    ItemLength::ItemLength(Item * addr, Item * dim) {
        this->itemtype = ItemType::item_length;
        this->dim = dim;
        this->addr = addr;
    } 


    /**
     * to_string for debug
     * */   

    std::string ItemTypeSig::to_string() {
        std::string ret;
        switch (this->vtype)
        {
        case VarType::code : {
            ret += "code";
            break;
        }
        
        case VarType::int64 : {
            ret += "int64";
            break;
        }

        case VarType::tuple : {
            ret += "tuple";
            break;
        }

        case VarType::tensor : {
            ret += "int64";
            for (int32_t i = 0; i < this->ndim; i++) {
                ret += "[]";
            }
            break;
        }
        
        case VarType::void_type : {
            ret += "void ";
            break;
        }

        default:
            std::cerr << "wrong op type in ItemTypeSig::to_string\n";
                assert(0);
                return "";
            }

        return ret;
    }

    std::string ItemConstant::to_string() {
        return std::to_string(this->constVal);

    }

    std::string ItemLabel::to_string() {
        return this->labelName;        
    }

    std::string ItemVariable::to_string() {
        return this->name;
    }
    
    std::string ItemArrAccess::to_string() {
        std::string ret = this->addr->to_string();
        for (Item * offset : this->offsets) {
            ret += "[";
            ret += offset->to_string();
            ret += "]";
        }

        return ret;
    }
    
    std::string ItemOp::to_string() {
        std::string ret;

        ret += this->op1->to_string();
        ret += " ";
        
        switch (this->opType)
        {
        case OpType::plus :
            ret += "+";
            break;

        case OpType::minus :
            ret += "-";
            break;

        case OpType::times :
            ret += "*";
            break;

        case OpType::bit_and :
            ret += "&";
            break;

        case OpType::shift_left :
            ret += "<<";
            break;
            
        case OpType::shift_right :
            ret += ">>";
            break;
        
        case OpType::eq :
            ret += "=";
            break;
        
        case OpType::less :
            ret += "<";
            break;
        
        case OpType::leq :
            ret += "<=";
            break;

        case OpType::great :
            ret += ">";
            break;

        case OpType::geq :
            ret += ">=";
            break;
        
        default:
            std::cerr << "wrong op type in ItemOp::to_string\n";
                assert(0);
                return "";
            
        }
        ret += " ";

        ret += this->op2->to_string(); 
        return ret;
    }


    std::string ItemCall::to_string() {
        std::string ret;
        ret += "call ";
        ret += this->callee->to_string();
        ret += "(";
        
        for (int32_t i = 0; i < this->args.size(); i++) {
            if (i > 0) {
                ret += ", ";
            }
            ret += this->args[i]->to_string();
        }
        
        ret += ")";
        return ret;
    }

    std::string ItemNewArray::to_string() {
        std::string ret = "new Array";
        ret += "(";
        
        for (int32_t i = 0; i < this->dims.size(); i++) {
            if (i > 0) {
                ret += ", ";
            }
            ret += this->dims[i]->to_string();
        }
        
        ret += ")";
        return ret;
    }

    std::string ItemNewTuple::to_string() {
        std::string ret = "new Tuple(";
        
        ret += this->len->to_string();
        ret += ")";
        
        return ret;
    }

    std::string ItemLength::to_string() {
        std::string ret = "length ";
        ret += this->addr->to_string();
        ret += " ";
        ret += this->dim->to_string();
        
        return ret;
    }

    /**
     * accept() for visitor
     * */
    void ItemTypeSig::accept(ItemVisitor &visitor) {
        visitor.visit(this);
    }

    void ItemConstant::accept(ItemVisitor & visitor){
        visitor.visit(this);
    }

    void ItemLabel::accept(ItemVisitor & visitor){
        visitor.visit(this);
    }

    void ItemVariable::accept(ItemVisitor & visitor){
        visitor.visit(this);
    }

    void ItemArrAccess::accept(ItemVisitor & visitor){
        visitor.visit(this);
    }

    void ItemOp::accept(ItemVisitor & visitor){
        visitor.visit(this);
    }
    
    void ItemCall::accept(ItemVisitor &visitor) {
        visitor.visit(this);
    }
    
    void ItemNewArray::accept(ItemVisitor &visitor) {
        visitor.visit(this);
    }

    void ItemNewTuple::accept(ItemVisitor &visitor) {
        visitor.visit(this);
    }
    
    void ItemLength::accept(ItemVisitor &visitor) {
        visitor.visit(this);
    }

    /**
     * copy() for "deepcopy"
     * */
    ItemTypeSig * ItemTypeSig::copy() {
        return this;
    }

    ItemConstant * ItemConstant::copy(){
        return new ItemConstant(this->constVal);
    }

    ItemLabel * ItemLabel::copy(){
        return new ItemLabel(this->labelName);
    }

    ItemVariable * ItemVariable::copy(){
        return this;
    }
    
    ItemArrAccess * ItemArrAccess::copy(){
        std::vector<Item *> offsets_cp (this->offsets.size());
        
        for (uint32_t i = 0; i < offsets_cp.size(); i++) {
            offsets_cp[i] = this->offsets[i]->copy();
        }
        
        return new ItemArrAccess(
            this->addr->copy(),
            offsets_cp
        );
    }


    ItemOp * ItemOp::copy(){
        ItemOp * op = new ItemOp(
            this->op1->copy(),
            this->op2->copy(),
            this->opType
        );
        return op;
    }


    ItemCall * ItemCall::copy() {
        std::vector<Item *> args_cp (this->args.size());

        for (uint32_t i = 0; i < args_cp.size(); i++) {
            args_cp[i] = this->args[i]->copy();
        }

        return new ItemCall(
            this->isRuntime,
            this->callee->copy(),
            args_cp
        );
    }

    ItemNewArray *ItemNewArray::copy() {
        std::vector<Item *> dims_cp (this->dims.size());
        
        for (uint32_t i = 0; i < dims_cp.size(); i++) {
            dims_cp[i] = this->dims[i]->copy();
        }

        ItemNewArray *new_arr_copy = new ItemNewArray(
            dims_cp
        );

        return new_arr_copy;
    }

    ItemNewTuple * ItemNewTuple::copy() {

        ItemNewTuple *new_tup_copy = new ItemNewTuple(
            this->len->copy()
        );

        return new_tup_copy;
    }

    ItemLength * ItemLength::copy() {
        ItemLength * new_length = new ItemLength(
            this->addr->copy(),
            this->dim->copy()
        );
        
        return new_length;
    }

    ItemTypeSig tupleSig(VarType::tuple);
    ItemTypeSig codeSig(VarType::code);
    ItemTypeSig int64Sig(VarType::int64);
    ItemTypeSig voidSig(VarType::void_type);

    ItemLabel print_label(IR::print_str);
    ItemLabel input_label(IR::input_str);
    ItemLabel tensor_label(IR::tensor_str);

    std::set<ItemType> basicTypes = {
        ItemType::item_variable,
        ItemType::item_constant,
        ItemType::item_labels
    };

    std::set<ItemType> varAndConst = {
        ItemType::item_variable,
        ItemType::item_constant
    };

    std::set<ItemType> varAndLabel = {
        ItemType::item_variable,
        ItemType::item_labels
    };


    bool isRuntimeLabel(Item * item) {
        return  item == &print_label
            ||  item == &input_label
            ||  item == &tensor_label;
    }

    bool isBasicItem(Item * item) {
        return  item->itemtype != item_labels
            ||  item->itemtype != item_variable
            ||  item->itemtype != item_constant;
    }

    /**
     *  Constructors for instructions
     * */

    Instruction_label::Instruction_label(Item * label) {
        this->type = InstType::inst_label;
        this->item_label = label;
    }
    
    Instruction_ret::Instruction_ret() {
        this->type = InstType::inst_ret;
    }

    Instruction_ret_var::Instruction_ret_var(Item * value_to_return) {
        this->type = InstType::inst_ret_var;
        this->valueToReturn = value_to_return;
    }


    Instruction_branch::Instruction_branch(Item * dst) {
        this->type = InstType::inst_branch;
        this->dst = dst;
    }

    Instruction_branch_cond::Instruction_branch_cond(Item * dst1, Item * dst2, Item * condition) {
        this->type = InstType::inst_branch_cond;
        this->dst1 = dst1;
        this->dst2 = dst2;
        this->condition = condition;
    }
    
    Instruction_declare::Instruction_declare(Item * typeSig, Item * var) {
        this->type = InstType::inst_declare;
        this->typeSig = typeSig;
        this->var = var;
    }

    Instruction_call::Instruction_call(Item * call_wrap) {
        this->type = InstType::inst_call;
        this->call_wrap = call_wrap;
    }

    Instruction_assignment::Instruction_assignment(Item *src, Item *dst){
        this->type = InstType::inst_assign;
        this->src = src;
        this->dst = dst;
    }

    /**
     * Instruction to_string() for debugging
     * */

    std::string Instruction_label::to_string() {
        return this->item_label->to_string() + '\n';
    }

    std::string Instruction_ret::to_string() {        
        return "return\n";
    }
    
    std::string Instruction_ret_var::to_string() {
        std::string ret = "return ";
        ret += this->valueToReturn->to_string();
        ret += "\n";
        return ret;
    }

    std::string Instruction_branch::to_string() {
        std::string ret = "br ";

        ret += this->dst->to_string();

        ret += "\n";
        return ret;
    }
    
    std::string Instruction_branch_cond::to_string() {
        std::string ret = "br ";

        ret += this->condition->to_string();
        ret += " ";

        ret += this->dst1->to_string();
        ret += " ";

        ret += this->dst2->to_string();

        ret += "\n";
        return ret;
    }
    
    std::string Instruction_declare::to_string() {
        std::string ret = "";

        ret += this->typeSig->to_string();
        ret += " ";
        
        ret += this->var->to_string();
        ret += "\n";
        
        return ret;
    }
    
    
    std::string Instruction_call::to_string() {
        std::string ret = "";

        ret += this->call_wrap->to_string();
        ret += '\n';

        return ret;
    }
    
    
    std::string Instruction_assignment::to_string() {
        
        std::string dst = this->dst->to_string();
        std::string src = this->src->to_string();

        std::string ret = "";
        ret += dst;
        ret += " <- ";
        ret += src;
        ret += "\n";

        return ret;
        
    }
    
    /**
     * Instruction accept() 
     * */
    
    void Instruction_label::accept(InstVisitor &visitor) {
        visitor.visit(this);
    }

    void Instruction_ret::accept(InstVisitor & visitor) {
        visitor.visit(this);
    }

    void Instruction_ret_var::accept(InstVisitor & visitor) {
        visitor.visit(this);
    }

    void Instruction_branch::accept(InstVisitor &visitor) {
        visitor.visit(this);
    }
    
    void Instruction_branch_cond::accept(InstVisitor &visitor) {
        visitor.visit(this);
    }

    void Instruction_declare::accept(InstVisitor &visitor) {
        visitor.visit(this);
    }

    void Instruction_call::accept(InstVisitor &visitor) {
        visitor.visit(this);
    }

    void Instruction_assignment::accept(InstVisitor &visitor) {
        visitor.visit(this);
    }

    /**
     *  copy for instructions
     * */

    Instruction_label * Instruction_label::copy() {
        Instruction_label * inst_label = new Instruction_label(
            this->item_label->copy()
        );
    
        return inst_label;
    }
    
    Instruction_ret * Instruction_ret::copy() {
        Instruction_ret * ret = new Instruction_ret();
        return ret;
    }

    Instruction_ret_var * Instruction_ret_var::copy() {
        Instruction_ret_var * ret = new Instruction_ret_var(
            this->valueToReturn->copy()
        );
        
        return ret;
    }

    Instruction_branch * Instruction_branch::copy() {
        return new Instruction_branch(
            this->dst->copy()
        );
    }

    Instruction_branch_cond * Instruction_branch_cond::copy() {
        return new Instruction_branch_cond(
            this->dst1->copy(),
            this->dst2->copy(),
            this->condition->copy()
        );
    }

    Instruction_declare * Instruction_declare::copy()
    {
        Instruction_declare * declare = new Instruction_declare(
            this->typeSig->copy(),
            this->var->copy()
        );

        return declare;
    }
    
    Instruction_call *Instruction_call::copy()
    {
        Instruction_call *call = new Instruction_call(
            this->call_wrap->copy()
        );

        return call;
    }

    Instruction_assignment *Instruction_assignment::copy()
    {
        Instruction_assignment *assign = new Instruction_assignment(
            this->src->copy(),
            this->dst->copy()
        );

        return assign;
    }

    
    std::set<BasicBlock *> Instruction_ret::getSuccessor(
        std::map<ItemLabel *, BasicBlock *> & label2BB
    ) {
        return std::set<BasicBlock *>();
    }

    std::set<BasicBlock *> Instruction_ret_var::getSuccessor(
        std::map<ItemLabel *, BasicBlock *> & label2BB
    ) {
        return std::set<BasicBlock *>();
    }
    
    std::set<BasicBlock *> Instruction_branch::getSuccessor(
        std::map<ItemLabel *, BasicBlock *> & label2BB
    ) {
        std::set<BasicBlock *> succ;
        
        assert(this->dst->itemtype == ItemType::item_labels);
        ItemLabel * lb = (ItemLabel *) this->dst;

        assert(IN_SET(label2BB, lb));
        succ.insert(label2BB[lb]);

        return succ;
    }

    std::set<BasicBlock *> Instruction_branch_cond::getSuccessor(
        std::map<ItemLabel *, BasicBlock *> & label2BB
    ) { 
        std::set<BasicBlock *> succ;
        
        assert(this->dst1->itemtype == ItemType::item_labels);
        ItemLabel * lb1 = (ItemLabel *) this->dst1;
        assert(IN_SET(label2BB, lb1));
        succ.insert(label2BB[lb1]);

        assert(this->dst2->itemtype == ItemType::item_labels);
        ItemLabel * lb2 = (ItemLabel *) this->dst2;
        assert(IN_SET(label2BB, lb2));
        succ.insert(label2BB[lb2]);

        return succ;
    }
    
    void BasicBlock::print() {
        DEBUG_OUT << "\n";

        DEBUG_OUT << this->label->to_string();

        for (Instruction_normal * inst : this->insts ) {
            DEBUG_OUT << inst->to_string();
        }

        DEBUG_OUT << this->te->to_string();
        DEBUG_OUT << "\n";
    }

    /**
     * Function 
     * */

    // Function::Function() {
    //     this->name = NULL;
    //     // this->arguments = 0;
    //     // this->locals = 0;
    //     this->instructions = std::vector<Instruction *>();
    //     this->labelName2ptr = std::unordered_map<std::string, Item *>();
    //     this->varName2ptr = std::unordered_map<std::string, Item *>();
    // }

    void Function::print() {
        std::string header = "define ";
        header += this->retType->to_string();
        header += " ";
        header += this->name->to_string();
        header +=  "(";

        
        for (int32_t i = 0; i < this->arg_list.size(); i++) {
            if (i > 0) {
                header += ", ";
            }
            ItemVariable * var = (ItemVariable *) this->arg_list[i];
            header += var->typeSig->to_string();
            header +=  " ";
            header += var->to_string(); 
        }

        header +=  ")";
        header +=  "{\n";
        DEBUG_OUT << header;

        for (BasicBlock * BB : this->BasicBlocks) {
            BB->print();
        }

        DEBUG_OUT << "}\n";
    }

    // Function * Function::copy() {
    //     Function * cp = new Function();
    //     cp->name = this->name;
    //     // cp->arguments = this->arguments;
    //     // cp->locals = this->locals;
        
    //     /**
    //      *  copy instructions
    //      * */
    //     cp->instructions.clear();
    //     for (Instruction * inst : this->instructions) {
    //         cp->instructions.push_back(inst->copy());
    //     }
        

    //     cp->varName2ptr = this->varName2ptr;
    //     cp->Instlabels = this->Instlabels;
    //     cp->labelName2ptr = this->labelName2ptr;

    //     return cp;
    // }

    void Function::populatePredsSuccs() {
        /**
         *  buid label to BasicBlock map
         * */
        std::map<ItemLabel *, BasicBlock *> label2BB;
        for (BasicBlock * BB: this->BasicBlocks) {
            Item * lb = BB->label->item_label;
            assert(lb->itemtype == ItemType::item_labels);
            label2BB[(ItemLabel * ) lb] = BB;
        }

        for (BasicBlock * BB: this->BasicBlocks) {

            /**
             *  get successor based on terminator instruction
             * */
            Instruction_terminator * TE = BB->te;
            BB->succs = TE->getSuccessor(label2BB);
            
            /**
             *  for every successor assign current BB as preds
             * */
            for (BasicBlock * succ : BB->succs) {
                succ->preds.insert(BB);
            }
        }

    }

    void Program::populatePredsSuccs () {
        for (Function * F : this->functions) {
            F->populatePredsSuccs();
        }
    }
}