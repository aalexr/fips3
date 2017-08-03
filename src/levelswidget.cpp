#include <levelswidget.h>

LevelsWidget::LevelsWidget(QWidget* parent): QWidget(parent) {
	std::unique_ptr<QSlider> min_slider{new QSlider(Qt::Horizontal, this)};
	std::unique_ptr<QSlider> max_slider{new QSlider(Qt::Horizontal, this)};

	std::unique_ptr<QDoubleSpinBox> min_spinbox{new QDoubleSpinBox(this)};
	std::unique_ptr<QDoubleSpinBox> max_spinbox{new QDoubleSpinBox(this)};

	std::unique_ptr<QGridLayout> layout{new QGridLayout(this)};
	layout->addWidget(min_slider .release(), 0, 0);
	layout->addWidget(min_spinbox.release(), 0, 1);
	layout->addWidget(max_slider .release(), 1, 0);
	layout->addWidget(max_spinbox.release(), 1, 1);
	setLayout(layout.release());
}