#include "AbstractFilter.h"

void AbstractFilterSlotMachine::inputDescriptionAboutToChange(AbstractDataSource* source)
{
	d_parent->inputDescriptionAboutToChange(source);
}
void AbstractFilterSlotMachine::inputDescriptionChanged(AbstractDataSource* source)
{
	d_parent->inputDescriptionChanged(source);
}
void AbstractFilterSlotMachine::inputPlotDesignationAboutToChange(AbstractDataSource* source)
{
	d_parent->inputPlotDesignationAboutToChange(source);
}
void AbstractFilterSlotMachine::inputPlotDesignationChanged(AbstractDataSource* source)
{
	d_parent->inputPlotDesignationChanged(source);
}
void AbstractFilterSlotMachine::inputDataAboutToChange(AbstractDataSource* source)
{
	d_parent->inputDataAboutToChange(source);
}
void AbstractFilterSlotMachine::inputDataChanged(AbstractDataSource* source)
{
	d_parent->inputDataChanged(source);
}
void AbstractFilterSlotMachine::inputAboutToBeReplaced(AbstractDataSource* source, AbstractDataSource* replacement)
{
	d_parent->inputAboutToBeReplaced(source, replacement);
}

bool AbstractFilter::input(int port, AbstractDataSource *source)
{
	if (port<0 || (numInputs()>=0 && port>=numInputs())) return false;
	if (d_inputs.size() <= port) d_inputs.resize(port+1);
	if (d_inputs[port]) { // disconnect the old input
		d_inputs[port]->disconnect(SIGNAL(descriptionAboutToChange(AbstractDataSource*)), &d_slot_machine);
		d_inputs[port]->disconnect(SIGNAL(descriptionChanged(AbstractDataSource*)), &d_slot_machine);
		d_inputs[port]->disconnect(SIGNAL(plotDesignationAboutToChange(AbstractDataSource*)), &d_slot_machine);
		d_inputs[port]->disconnect(SIGNAL(plotDesignationChanged(AbstractDataSource*)), &d_slot_machine);
		d_inputs[port]->disconnect(SIGNAL(dataAboutToChange(AbstractDataSource*)), &d_slot_machine);
		d_inputs[port]->disconnect(SIGNAL(dataChanged(AbstractDataSource*)), &d_slot_machine);
		d_inputs[port]->disconnect(SIGNAL(aboutToBeReplaced(AbstractDataSource*,AbstractDataSource*)), &d_slot_machine);
	}
	inputDescriptionAboutToChange(d_inputs[port]);
	inputPlotDesignationAboutToChange(d_inputs[port]);
	inputDataAboutToChange(d_inputs[port]);
	d_inputs[port] = source;
	inputDescriptionChanged(source);
	inputPlotDesignationChanged(source);
	inputDataChanged(source);
	QObject::connect(source, SIGNAL(descriptionAboutToChange(AbstractDataSource*)),
			&d_slot_machine, SLOT(inputDescriptionAboutToChange(AbstractDataSource*)));
	QObject::connect(source, SIGNAL(descriptionChanged(AbstractDataSource*)),
			&d_slot_machine, SLOT(inputDescriptionChanged(AbstractDataSource*)));
	QObject::connect(source, SIGNAL(plotDesignationAboutToChange(AbstractDataSource*)),
			&d_slot_machine, SLOT(inputPlotDesignationAboutToChange(AbstractDataSource*)));
	QObject::connect(source, SIGNAL(plotDesignationChanged(AbstractDataSource*)),
			&d_slot_machine, SLOT(inputPlotDesignationChanged(AbstractDataSource*)));
	QObject::connect(source, SIGNAL(dataAboutToChange(AbstractDataSource*)),
			&d_slot_machine, SLOT(inputDataAboutToChange(AbstractDataSource*)));
	QObject::connect(source, SIGNAL(dataChanged(AbstractDataSource*)),
			&d_slot_machine, SLOT(inputDataChanged(AbstractDataSource*)));
	QObject::connect(source, SIGNAL(aboutToBeReplaced(AbstractDataSource*,AbstractDataSource*)),
			&d_slot_machine, SLOT(inputAboutToBeReplaced(AbstractDataSource*,AbstractDataSource*)));
	return true;
}

bool AbstractFilter::input(AbstractFilter* sources)
{
	if (!sources) return false;
	bool result = true;
	for (int i=0; i<sources->numOutputs(); i++)
		if (!input(i, sources->output(i)))
			result = false;
	return result;
}

QString AbstractFilter::inputLabel(int port) const
{
	return QObject::tr("In%1").arg(port + 1);
}

void AbstractFilter::inputDescriptionAboutToChange(AbstractDataSource* source)
{
	inputDescriptionAboutToChange(d_inputs.indexOf(source));
}
void AbstractFilter::inputDescriptionChanged(AbstractDataSource* source)
{
	inputDescriptionChanged(d_inputs.indexOf(source));
}
void AbstractFilter::inputPlotDesignationAboutToChange(AbstractDataSource* source)
{
	inputPlotDesignationAboutToChange(d_inputs.indexOf(source));
}
void AbstractFilter::inputPlotDesignationChanged(AbstractDataSource* source)
{
	inputPlotDesignationChanged(d_inputs.indexOf(source));
}
void AbstractFilter::inputDataAboutToChange(AbstractDataSource* source)
{
	inputDataAboutToChange(d_inputs.indexOf(source));
}
void AbstractFilter::inputDataChanged(AbstractDataSource* source)
{
	inputDataChanged(d_inputs.indexOf(source));
}
void AbstractFilter::inputAboutToBeReplaced(AbstractDataSource* source, AbstractDataSource* replacement)
{
	input(d_inputs.indexOf(source), replacement);
}

