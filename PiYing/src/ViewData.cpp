#include "ViewData.h"

void ViewData::setValue(float v)
{
	if (m_value != v) m_value = v; emit valueChanged(v);
}
