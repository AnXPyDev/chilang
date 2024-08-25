void Simulator_create(Simulator *this, MemberList *global) {
    SimFrame_create(&this->globalFrame, this, global, NULL);
}

SimResult Simulator_evaluate(Simulator *this, Expression expression, SimValue **out_value) {
    return Expression_simulate(expression, this, (SimState) {
        .frame = &this->globalFrame
    }, out_value);
}