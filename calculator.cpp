#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QString>
#include <QStack>
#include <cmath>

class Calculator : public QWidget {
    Q_OBJECT

public:
    Calculator(QWidget *parent = 0);

private slots:
    void digitClicked();
    void operatorClicked();
    void equalsClicked();
    void clearDisplay();

private:
    QLineEdit *display;
    QString currentInput;
    double evaluateExpression(const QString &expression);
};

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), currentInput("") {

    // Create the display
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(15);

    // Create buttons
    const char* buttonLabels[4][4] = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"0", "C", "=", "+"}
    };

    QGridLayout *gridLayout = new QGridLayout(this);
    gridLayout->addWidget(display, 0, 0, 1, 4);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            QPushButton *button = new QPushButton(buttonLabels[i][j], this);

            if (buttonLabels[i][j] == "=") {
                connect(button, SIGNAL(clicked()), this, SLOT(equalsClicked()));
                button->setStyleSheet("background-color: orange");
            } else if (buttonLabels[i][j] == "C") {
                connect(button, SIGNAL(clicked()), this, SLOT(clearDisplay()));
                button->setStyleSheet("background-color: red");
            } else {
                connect(button, SIGNAL(clicked()), this, SLOT(digitClicked()));
            }

            gridLayout->addWidget(button, i + 1, j);
        }
    }

    setLayout(gridLayout);
}

void Calculator::digitClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    currentInput += clickedButton->text();
    display->setText(currentInput);
}

void Calculator::operatorClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    currentInput += " " + clickedButton->text() + " ";
    display->setText(currentInput);
}

double Calculator::evaluateExpression(const QString &expression) {
    QStack<double> values;
    QStack<QChar> operators;

    auto applyOperator = [&]() {
        double b = values.pop();
        double a = values.pop();
        QChar op = operators.pop();
        switch (op.toLatin1()) {
            case '+': values.push(a + b); break;
            case '-': values.push(a - b); break;
            case '*': values.push(a * b); break;
            case '/': values.push(a / b); break;
        }
    };

    QStringList tokens = expression.split(' ', Qt::SkipEmptyParts);

    for (const QString &token : tokens) {
        if (token == "+" || token == "-") {
            while (!operators.isEmpty() && (operators.top() == '+' || operators.top() == '-' || operators.top() == '*' || operators.top() == '/')) {
                applyOperator();
            }
            operators.push(token.at(0));
        } else if (token == "*" || token == "/") {
            while (!operators.isEmpty() && (operators.top() == '*' || operators.top() == '/')) {
                applyOperator();
            }
            operators.push(token.at(0));
        } else {
            values.push(token.toDouble());
        }
    }

    while (!operators.isEmpty()) {
        applyOperator();
    }

    return values.pop();
}

void Calculator::equalsClicked() {
    try {
        QString result = QString::number(evaluateExpression(currentInput));
        display->setText(result);
        currentInput = result;
    } catch (...) {
        display->setText("Error");
        currentInput = "";
    }
}

void Calculator::clearDisplay() {
    currentInput = "";
    display->clear();
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Calculator calc;
    calc.setWindowTitle("Colorful Calculator");
    calc.resize(400, 400);
    calc.show();
    return app.exec();
}