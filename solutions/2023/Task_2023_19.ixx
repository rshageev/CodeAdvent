export module AoC_2023.Day19;

import std;
import utils;

export int Solve_1(const std::filesystem::path& input);
export size_t Solve_2(const std::filesystem::path& input);

module : private;

using Part = std::array<int, 4>;

struct Rule
{
    std::string to;
    int param_idx = 0;
    int cond_value = 0;
    char cond = 0;
};

using Workflow = std::vector<Rule>;
using WorkflowMap = std::unordered_map<std::string, Workflow>;


constexpr Rule ParseRule(std::string_view str)
{
    auto p = str.find(':');
    if (p == std::string_view::npos) {
        return { std::string(str) };
    }

    std::string_view to = str.substr(p + 1);

    const int param_idx = std::string_view("xmas").find(str[0]);
    const char cond = str[1];

    str.remove_prefix(2);
    const int cv = Read<int>(str);

    return { std::string(to), param_idx, cv, cond };
}

std::pair<std::string, Workflow> ParseWorkflow(std::string_view str)
{
    std::pair<std::string, Workflow> out;

    out.first = ReadWord(str, "{");

    str.remove_prefix(1); // remove brackets
    str.remove_suffix(1);

    for (auto rule : str | stdv::split(',')){
        out.second.push_back(ParseRule(std::string_view(rule)));
    }

    return out;
}

Part ParsePart(std::string_view str)
{
    Part part;

    str.remove_prefix(1); // remove {

    for (auto [idx, part_str] : str | stdv::split(',') | stdv::transform(make<std::string_view>) | stdv::enumerate) {
        part_str.remove_prefix(2);
        part[idx] = Read<int>(part_str);
    }

    return part;
}

auto ReadInput(const std::filesystem::path& input)
{
    std::pair<WorkflowMap, std::vector<Part>> out;

    bool parse_workflows = true;
    for (const auto& line : ReadLines(input))
    {
        if (!line.empty()) {
            if (parse_workflows) {
                out.first.insert(ParseWorkflow(line));
            } else {
                out.second.push_back(ParsePart(line));
            }
        } else {
            parse_workflows = false;
        }
    }
    return out;
}

bool CheckPart(const Part& part, const Rule& rule)
{
    switch (rule.cond) {
        case '>': return part[rule.param_idx] > rule.cond_value;
        case '<': return part[rule.param_idx] < rule.cond_value;
        default: return true;
    }
}

std::string CheckPart(const Part& part, const Workflow& w)
{
    for (const auto& rule : w) {
        if (CheckPart(part, rule)) {
            return rule.to;
        }
    }
    return "R";
}

bool CheckPart(const Part& part, const WorkflowMap& wmap)
{
    std::string next = "in";
    while (next != "A" && next != "R") {
        const auto& w = wmap.at(next);
        next = CheckPart(part, w);
    }
    return (next == "A");
}

int Solve_1(const std::filesystem::path& input)
{
    const auto [workflows, parts] = ReadInput(input);

    int sum = 0;
    for (const auto& part : parts)
    {
        if (CheckPart(part, workflows))
        {
            sum += (part[0] + part[1] + part[2] + part[3]);
        }
    }

    return sum;
}

using PartRange = std::array<std::pair<int, int>, 4>;

constexpr PartRange DefRange = {{ {1,4001}, {1,4001}, {1,4001}, {1, 4001} }};
constexpr PartRange EmptyRange = { { {0,0}, {0,0}, {0,0}, {0,0} } };

std::pair<PartRange, PartRange> SplitRange(const PartRange& pr, const Rule& rule)
{
    PartRange accepted = pr;
    PartRange rejected = pr;

    auto [in1, in2] = pr[rule.param_idx];

    if (rule.cond == '>') {
        accepted[rule.param_idx] = { std::clamp(rule.cond_value + 1, in1, in2), in2 };
        rejected[rule.param_idx] = { in1, std::clamp(rule.cond_value + 1, in1, in2) };
    } else if (rule.cond == '<') {
        accepted[rule.param_idx] = { in1, std::clamp(rule.cond_value, in1, in2) };
        rejected[rule.param_idx] = { std::clamp(rule.cond_value, in1, in2), in2 };
    } else {
        rejected = EmptyRange;
    }

    return { accepted, rejected };
}

size_t Combos(const PartRange& pr)
{
    return static_cast<size_t>(pr[0].second - pr[0].first)
        * static_cast<size_t>(pr[1].second - pr[1].first) 
        * static_cast<size_t>(pr[2].second - pr[2].first)
        * static_cast<size_t>(pr[3].second - pr[3].first);
}

size_t AcceptedCombos(const WorkflowMap& wmap)
{
    std::queue<std::tuple<std::string, size_t, PartRange>> q; // workflow, rule number, which part combos can reach that

    q.emplace("in", 0, DefRange);

    size_t result = 0;

    while (!q.empty())
    {
        auto [wf_name, rule_idx, parts_range] = q.front();
        const auto& wf = wmap.at(wf_name);
        const auto& rule = wf[rule_idx];

        auto [passed, rejected] = SplitRange(parts_range, rule);

        if (rule.to == "A") {    
            result += Combos(passed);
        } else if (rule.to != "R") {      
            q.emplace(rule.to, 0, passed);
        }

        if (rule_idx < wf.size() - 1) {
            q.emplace(wf_name, rule_idx + 1, rejected);
        }

        q.pop();
    }

    return result;
}

size_t Solve_2(const std::filesystem::path& input)
{
    const auto [workflows, parts] = ReadInput(input);
    return AcceptedCombos(workflows);
}