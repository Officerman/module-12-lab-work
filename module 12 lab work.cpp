#include <iostream>
#include <memory>
#include <string>

// Forward declaration
class BookingContext;

// Абстрактное состояние
class BookingState {
public:
    virtual ~BookingState() = default;
    virtual void handle(BookingContext& context, const std::string& action) = 0;
    virtual std::string getName() const = 0;
};

// Контекст, управляющий состояниями
class BookingContext {
private:
    std::unique_ptr<BookingState> state;
public:
    explicit BookingContext(std::unique_ptr<BookingState> initialState) : state(std::move(initialState)) {}

    void setState(std::unique_ptr<BookingState> newState) {
        state = std::move(newState);
    }

    void request(const std::string& action) {
        state->handle(*this, action);
    }

    std::string getStateName() const {
        return state->getName();
    }
};

// Конкретные состояния
class IdleState : public BookingState {
public:
    void handle(BookingContext& context, const std::string& action) override;
    std::string getName() const override { return "Idle"; }
};

class RoomSelectedState : public BookingState {
public:
    void handle(BookingContext& context, const std::string& action) override;
    std::string getName() const override { return "RoomSelected"; }
};

class BookingConfirmedState : public BookingState {
public:
    void handle(BookingContext& context, const std::string& action) override;
    std::string getName() const override { return "BookingConfirmed"; }
};

class PaidState : public BookingState {
public:
    void handle(BookingContext& context, const std::string& action) override;
    std::string getName() const override { return "Paid"; }
};

class BookingCancelledState : public BookingState {
public:
    void handle(BookingContext&, const std::string&) override {
        std::cout << "Booking already cancelled.\n";
    }
    std::string getName() const override { return "BookingCancelled"; }
};

// Реализация переходов между состояниями
void IdleState::handle(BookingContext& context, const std::string& action) {
    if (action == "select_room") {
        std::cout << "Room selected. Moving to RoomSelected state.\n";
        context.setState(std::make_unique<RoomSelectedState>());
    } else {
        std::cout << "Invalid action in Idle state.\n";
    }
}

void RoomSelectedState::handle(BookingContext& context, const std::string& action) {
    if (action == "confirm_booking") {
        std::cout << "Booking confirmed. Moving to BookingConfirmed state.\n";
        context.setState(std::make_unique<BookingConfirmedState>());
    } else if (action == "cancel") {
        std::cout << "Booking cancelled. Moving to BookingCancelled state.\n";
        context.setState(std::make_unique<BookingCancelledState>());
    } else {
        std::cout << "Invalid action in RoomSelected state.\n";
    }
}

void BookingConfirmedState::handle(BookingContext& context, const std::string& action) {
    if (action == "pay") {
        std::cout << "Payment completed. Moving to Paid state.\n";
        context.setState(std::make_unique<PaidState>());
    } else if (action == "cancel") {
        std::cout << "Booking cancelled. Moving to BookingCancelled state.\n";
        context.setState(std::make_unique<BookingCancelledState>());
    } else {
        std::cout << "Invalid action in BookingConfirmed state.\n";
    }
}

void PaidState::handle(BookingContext&, const std::string&) {
    std::cout << "Booking is already paid and completed.\n";
}

// Тестирование системы
int main() {
    BookingContext context(std::make_unique<IdleState>());

    std::cout << "Current state: " << context.getStateName() << "\n";

    context.request("select_room");
    std::cout << "Current state: " << context.getStateName() << "\n";

    context.request("confirm_booking");
    std::cout << "Current state: " << context.getStateName() << "\n";

    context.request("pay");
    std::cout << "Current state: " << context.getStateName() << "\n";

    return 0;
}
