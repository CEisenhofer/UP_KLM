#include "Program.h"
#include "language.tab.h"
#include <cstdlib>
#include <chrono>
#include <fstream>
#include <filesystem>

z3::context* g_ctx = nullptr;
z3::func_decl* g_node_fct = nullptr;
z3::expr_vector* g_subterms = nullptr;

unsigned parse_string(const char* in);

static decltype(std::chrono::high_resolution_clock::now()) start_time;
static decltype(std::chrono::high_resolution_clock::now()) end_time;

static void start_watch() {
    start_time = std::chrono::high_resolution_clock::now();
}

static uint64_t stop_watch() {
    end_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
}

static std::unordered_map<std::string, Logic> SupportedLogicsMap;

static void crash_params() {
    std::cout << "Usage: KLM [-t=[timeout in ms] -l=[C|CL|CM|P|M] --smtlib --visual] <file>" << std::endl;
    exit(-1);
}

static void
parse_params(const std::vector<std::string>& args, unsigned& timeout, Logic& logic, bool& smtlib, bool& visual,
             bool& checkResult, bool& bench) {
    if (args.empty() || !std::filesystem::exists(args[args.size() - 1]))
        crash_params();

    SupportedLogicsMap["C"] = C;
    SupportedLogicsMap["CL"] = CL;
    SupportedLogicsMap["CM"] = CM;
    SupportedLogicsMap["P"] = P;
    SupportedLogicsMap["M"] = M;

    timeout = 0;
    logic = C;
    smtlib = false;
    visual = false;
    checkResult = true;
    bench = false;

    for (int i = 0; i < args.size() - 1; i++) {
        if (starts_with(args[i], "-t=")) {
            std::stringstream ss(args[i]);
            ss.ignore(3);
            ss >> timeout;
            if (ss.fail())
                crash_params();
            if (timeout <= 0)
                crash_params();
            continue;
        }
        if (starts_with(args[i], "-l=")) {
            const std::string logicString = args[i].substr(3);
            auto it = SupportedLogicsMap.find(logicString);
            if (it == SupportedLogicsMap.end())
                crash_params();
            logic = it->second;
            continue;
        }
        if (args[i] == "--bench") {
            bench = true;
        }
        if (args[i] == "--no-check") {
            checkResult = false;
        }
        if (args[i] == "--smtlib") {
            smtlib = true;
        }
        if (args[i] == "--visual") {
            visual = true;
        }
    }
}

#if 0
enum op : unsigned char {
    op_not,
    op_open,
    op_and,
    op_or,
    op_implies,
    op_eq,
    op_edge,
    op_none_edge,
    op_none,
    op_close,
};

enum char_type {
    char_ws,
    char_alphanumeric,
    char_single,
    char_open,
    char_close,
    other,
};

static inline bool is_alphanumeric(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_' || c == '$' ||
           c == '#';
}

static inline bool is_whitespace(char c) {
    return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

static inline char_type get_char_type(char c) {
    if (is_whitespace(c))
        return char_ws;
    if (is_alphanumeric(c))
        return char_alphanumeric;
    // These operators can be put directly after each other -> we have to consider them alone
    if (c == '!' || c == '(' || c == ')')
        return char_single;
    return other;
}

static expr parse_language(context &ctx, func_decl &node_fct, const std::string &line) {
    std::vector<op> operator_stack;
    expr_vector output_stack(ctx);
    std::stringstream tokenizer;
    operator_stack.push_back(op_open);

    auto shift = [&ctx, &node_fct, &line, &output_stack, &operator_stack, &tokenizer]() {
        std::string s = tokenizer.str();
        if (s.empty())
            return;
        tokenizer.str("");
        op op1;
        if (s == "true")
            output_stack.push_back(ctx.bool_val(true)), op1 = op_none;
        else if (s == "false")
            output_stack.push_back(ctx.bool_val(false)), op1 = op_none;
        else if (s == "!")
            op1 = op_not;
        else if (s == "&")
            op1 = op_and;
        else if (s == "|")
            op1 = op_or;
        else if (s == "=>")
            op1 = op_implies;
        else if (s == "=")
            op1 = op_eq;
        else if (s == "->")
            op1 = op_edge;
        else if (s == "-/>")
            op1 = op_none_edge;
        else if (s == "(")
            operator_stack.push_back(op_open), op1 = op_none;
        else if (s == ")")
            op1 = op_close;
        else
            output_stack.push_back(ctx.bool_const(s.c_str())), op1 = op_none;
        if (op1 == op_none)
            return;
        op op2;
        while (!operator_stack.empty() && operator_stack.back() != op_open && operator_stack.back() < op1) {
            op2 = operator_stack.back();
            operator_stack.pop_back();
            if (output_stack.empty())
                throw exception(("Missing operand in assertion " + line).c_str());
            if (op2 == op_not) {
                output_stack.push_back(output_stack.back());
                output_stack.pop_back();
                return;
            }
            if (output_stack.size() < 2)
                throw exception(("Missing operand in assertion " + line).c_str());
            expr a1 = output_stack[output_stack.size() - 2];
            expr a2 = output_stack[output_stack.size() - 1];
            output_stack.pop_back();
            output_stack.pop_back();
            switch (op2) {
                case op_and:
                    output_stack.push_back(a1 && a2);
                    break;
                case op_or:
                    output_stack.push_back(a1 || a2);
                    break;
                case op_implies:
                    output_stack.push_back(implies(a1, a2));
                    break;
                case op_eq:
                    output_stack.push_back(a1 == a2);
                    break;
                case op_edge:
                    output_stack.push_back(node_fct(a1, a2));
                    break;
                case op_none_edge:
                    output_stack.push_back(!node_fct(a1, a2));
                    break;
            }
        }
        if (op1 == op_close) {
            if (operator_stack.empty() || operator_stack.back() != op_open)
                throw exception(("Unmatched bracket in assertion " + line).c_str());
            operator_stack.pop_back();
        } else
            operator_stack.push_back(op1);
    };

    char_type prev = char_ws;
    for (char i: line) {
        const char_type curr = get_char_type(i);
        if (prev != char_ws && (prev == char_single || curr != prev))
            shift();
        if (curr != char_ws)
            tokenizer << i;
        prev = curr;
    }
    shift();
    tokenizer << ")"; // force evaluation of everything!
    shift();
    if (!operator_stack.empty() || output_stack.size() != 1)
        throw exception(("Missing operand in assertion " + line).c_str());
    return output_stack.back();
}
#else

static expr parse_language(context& ctx, func_decl& node_fct, const std::string& line) {
    g_ctx = &ctx;
    g_node_fct = &node_fct;
    z3::expr_vector subterms(ctx);
    g_subterms = &subterms;
    unsigned i = parse_string(line.c_str());
    expr e = subterms[i];
    assert(i == subterms.size() - 1);
    g_ctx = nullptr;
    g_node_fct = nullptr;
    g_subterms = nullptr;
    return e;
}

#endif

int main(int argc, char** argv) {
    std::vector<std::string> args(argc);
    for (unsigned i = 0; i < argc; i++) {
        args[i] = argv[i];
    }
    unsigned timeout;
    Logic logic;
    bool smtlib;
    bool visual;
    bool checkResult;
    bool bench;
    parse_params(args, timeout, logic, smtlib, visual, checkResult, bench);

    if (bench) {
        benchmark(logic);
        return 0;
    }

    context ctx;
    std::string smtlib2;
    std::string content;
    try {
        std::ifstream t(args[args.size() - 1]);
        std::stringstream buffer;
        buffer << t.rdbuf();
        content = buffer.str();
    }
    catch (std::exception& e) {
        std::cout << "Could not open file: " << args[0] << std::endl;
        exit(-1);
    }
    if (smtlib) {
        smtlib2 = content;
    }
    else {
        expr_vector assertions(ctx);
        try {
            sort_vector domain(ctx);
            domain.push_back(ctx.bool_sort());
            domain.push_back(ctx.bool_sort());
            func_decl node_fct = ctx.function("pair", domain, ctx.bool_sort());
            std::stringstream ss(content);
            std::string line;
            while (std::getline(ss, line, '\n')) {
                assertions.push_back(parse_language(ctx, node_fct, line));
            }
        }
        catch (std::exception& e) {
            exit(-1);
        }
        smtlib2 = to_SMTLIB(assertions);
    }
    Solve(ctx, smtlib2, timeout, logic, smtlib, visualn
    checkResult);
    return 0;
}

void
Solve(context& context, const std::string& smtlib2, unsigned timeout, Logic logic, bool smtlibOutput, bool checkResult,
      bool visual) {
    sort nodeSort = context.uninterpreted_sort("Node");

    sort_vector domain(context);
    domain.push_back(context.bool_sort());
    domain.push_back(context.bool_sort());
    func_decl nodeFct = context.function("pairReal", domain, context.bool_sort());

    domain.resize(0);
    domain.push_back(nodeSort);
    domain.push_back(nodeSort);
    func_decl nodeFctAbstr = context.user_propagate_function(
            context.str_symbol("pair"), domain, context.bool_sort());

    sort_vector empty_sorts(context);
    func_decl_vector node_decl(context);
    node_decl.push_back(nodeFctAbstr);

    Z3_symbol decl_names[1];
    decl_names[0] = Z3_mk_string_symbol(context, "pair");
    Z3_func_decl decl_fcts[1];
    decl_fcts[0] = nodeFct;

    auto parsed = Z3_parse_smtlib2_string(context, smtlib2.c_str(), 0, nullptr, nullptr, 1, decl_names, decl_fcts);
    if (Z3_get_error_code(context) != Z3_error_code::Z3_OK)
        throw z3::exception(Z3_get_error_msg(context, Z3_get_error_code(context)));

    expr assertion = mk_and(expr_vector(context, parsed));

    std::unordered_map<expr, expr_template*, expr_hash, expr_eq> abstraction;
    std::unordered_map<expr, std::optional<expr>, expr_hash, expr_eq> abstractionInv;

    assertion = replace_nodes(assertion, nodeFct, nodeFctAbstr, nodeSort, abstraction, abstractionInv);

    params param(context);
    param.set("smt.phase_selection", 1u);
    param.set("solver.timeout", timeout);

    solver solver(context);
    solver.set(param);

    start_watch();
    KLMPropagator propagator(solver, nodeFctAbstr, logic, abstraction);
    std::cout << assertion << std::endl;
    solver.add(assertion);

    const check_result res = solver.check();
    const auto time = stop_watch();
    std::cout << res << " in " << time << "mrs" << std::endl;
    if (res == sat) {
        std::cout << std::endl;
        std::string model = propagator.get_model(smtlibOutput);
        std::cout << model << std::endl;
        if (visual) {
            std::string s = propagator.display_model(smtlibOutput);
            std::ofstream of("output-graph.dot");
            of << s;
            of.close();
            std::ofstream of2("output-info.txt");
            of2 << model;
            of2.close();
            std::cout
                    << "Outputting graph to \"output-graph.dot\" - use dot (graphviz) to compile.\nModel written to \"output-info.txt\""
                    << std::endl;
        }
        propagator.check_model();
    }
}


void get_constants(expr e, std::unordered_set<expr, expr_hash, expr_eq>& names) {
    if (e.decl().decl_kind() == Z3_OP_UNINTERPRETED && e.num_args() == 0) {
        names.insert(e);
        return;
    }
    for (const auto& arg: e.args()) {
        get_constants(arg, names);
    }
}

std::string to_SMTLIB(const expr_vector& assertions) {
    expr conj = assertions.size() == 1 ? assertions[0] : mk_and(assertions);
    std::unordered_set<expr, expr_hash, expr_eq> constants;
    get_constants(conj, constants);
    std::stringstream sb;
    for (const auto& constant: constants) {
        sb << "(declare-const " << constant.decl().name().str() << " Bool)\n";
    }

    for (const auto& assertion: assertions) {
        sb << "(assert " << assertion << ")\n";
    }
    return sb.str();
}

void benchmark(Logic logic) {
    std::random_device rd;
    int seed = 0;
    std::cout << "Random seed: " << seed << std::endl;
    constexpr int maxVariables = 10;
    RandomFormula randomFormula(seed, maxVariables, 2, 10, 10);
    for (int i = 0; i < 100; i++) {
        context ctx;
        expr_vector assertions(ctx);
        Specification spec = randomFormula.gen_spec();
        sort_vector domain(ctx);
        domain.push_back(ctx.bool_sort());
        domain.push_back(ctx.bool_sort());
        func_decl nodeFct = ctx.function("pair", domain, ctx.bool_sort());
        for (const auto& pos: spec.get_positive()) {
            auto lhs = pos->get_lhs();
            auto rhs = pos->get_rhs();
            assertions.push_back(nodeFct(lhs->ToZ3(ctx).simplify(), rhs->ToZ3(ctx).simplify()));
        }
        assertions.push_back(
                !nodeFct(spec.get_negative()->get_lhs()->ToZ3(ctx).simplify(),
                         spec.get_negative()->get_rhs()->ToZ3(ctx).simplify()));
        Solve(ctx, to_SMTLIB(assertions), 0, logic, false, false);
    }
}

expr create_template(expr e, std::unordered_map<std::string, std::pair<unsigned, std::optional<expr>>>& variables) {
    if (e.decl().decl_kind() == Z3_OP_UNINTERPRETED && e.num_args() == 0 && e.is_bool()) {
        std::string name = e.decl().name().str();
        const auto it = variables.find(name);
        if (it != variables.end())
            return *it->second.second;
        expr varRes = e.ctx().variable((unsigned) variables.size(), e.ctx().bool_sort());
        variables[name] = std::make_pair(variables.size(), varRes);
        return varRes;
    }
    expr_vector args(e.ctx());
    for (unsigned i = 0; i < e.num_args(); i++) {
        args.push_back(create_template(e.arg(i), variables));
    }
    return e.decl()(args);
}

expr replace_nodes(const expr& e, const func_decl& oldDecl, const func_decl& newDecl, const sort& nodeSort,
                   std::unordered_map<expr, expr_template*, expr_hash, expr_eq>& abstraction,
                   std::unordered_map<expr, std::optional<expr>, expr_hash, expr_eq>& abstractionInv) {
    if (Z3_is_eq_func_decl(e.ctx(), e.decl(), oldDecl)) {
        expr arg1 = e.arg(0).simplify();
        expr arg2 = e.arg(1).simplify();
        auto it = abstractionInv.find(arg1);
        expr abstr1(e.ctx()), abstr2(e.ctx());
        if (it == abstractionInv.end()) {
            abstr1 = e.ctx().constant(arg1.to_string().c_str(), nodeSort);
            abstractionInv[arg1] = abstr1;
            std::unordered_map<std::string, std::pair<unsigned, std::optional<expr>>> variables;
            const auto templateExpr = create_template(arg1, variables);
            std::vector<std::string> variableNames = std::vector<std::string>(variables.size());
            for (const auto& variable: variables) {
                variableNames[variable.second.first] = variable.first;
            }
            abstraction[abstr1] = new expr_template(templateExpr, std::move(variableNames));
        }
        else
            abstr1 = *it->second;

        it = abstractionInv.find(arg2);

        if (it == abstractionInv.end()) {
            abstr2 = e.ctx().constant(arg2.to_string().c_str(), nodeSort);
            abstractionInv[arg2] = abstr2;
            std::unordered_map<std::string, std::pair<unsigned, std::optional<expr>>> variables;
            auto templateExpr = create_template(arg2, variables);
            std::vector<std::string> variableNames = std::vector<std::string>(variables.size());
            for (const auto& variable: variables) {
                variableNames[variable.second.first] = variable.first;
            }
            abstraction[abstr2] = new expr_template(templateExpr, std::move(variableNames));
        }
        else
            abstr2 = *it->second;

        return newDecl(abstr1, abstr2);
    }
    expr_vector args(e.ctx());
    for (const auto& arg: e.args()) {
        args.push_back(replace_nodes(arg, oldDecl, newDecl, nodeSort, abstraction, abstractionInv));
    }
    return e.decl()(args);
}
