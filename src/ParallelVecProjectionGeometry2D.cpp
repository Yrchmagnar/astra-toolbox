/*
-----------------------------------------------------------------------
Copyright: 2010-2018, iMinds-Vision Lab, University of Antwerp
           2014-2018, CWI, Amsterdam

Contact: astra@astra-toolbox.com
Website: http://sf.net/projects/astra-toolbox

This file is part of the ASTRA Toolbox.


The ASTRA Toolbox is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The ASTRA Toolbox is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with the ASTRA Toolbox. If not, see <http://www.gnu.org/licenses/>.

-----------------------------------------------------------------------
$Id$
*/

#include "astra/ParallelVecProjectionGeometry2D.h"

#include <cstring>
#include <sstream>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace astra
{

//----------------------------------------------------------------------------------------
// Default constructor. Sets all variables to zero. 
CParallelVecProjectionGeometry2D::CParallelVecProjectionGeometry2D()
{
	_clear();
	m_pProjectionAngles = 0;
}

//----------------------------------------------------------------------------------------
// Constructor.
CParallelVecProjectionGeometry2D::CParallelVecProjectionGeometry2D(int _iProjectionAngleCount, 
                                                                 int _iDetectorCount, 
                                                                 const SParProjection* _pProjectionAngles)
{
	this->initialize(_iProjectionAngleCount, 
	                 _iDetectorCount, 
	                 _pProjectionAngles);
}

//----------------------------------------------------------------------------------------
// Copy Constructor
CParallelVecProjectionGeometry2D::CParallelVecProjectionGeometry2D(const CParallelVecProjectionGeometry2D& _projGeom)
{
	_clear();
	this->initialize(_projGeom.m_iProjectionAngleCount,
	                 _projGeom.m_iDetectorCount,
	                 _projGeom.m_pProjectionAngles);
}

//----------------------------------------------------------------------------------------
// Destructor.
CParallelVecProjectionGeometry2D::~CParallelVecProjectionGeometry2D()
{
	// TODO
	delete[] m_pProjectionAngles;
}


//----------------------------------------------------------------------------------------
// Initialization.
bool CParallelVecProjectionGeometry2D::initialize(int _iProjectionAngleCount, 
											  int _iDetectorCount, 
											  const SParProjection* _pProjectionAngles)
{
	m_iProjectionAngleCount = _iProjectionAngleCount;
	m_iDetectorCount = _iDetectorCount;
	m_pProjectionAngles = new SParProjection[m_iProjectionAngleCount];
	for (int i = 0; i < m_iProjectionAngleCount; ++i)
		m_pProjectionAngles[i] = _pProjectionAngles[i];

	// TODO: check?

	// success
	m_bInitialized = _check();
	return m_bInitialized;
}

//----------------------------------------------------------------------------------------
// Initialization with a Config object
bool CParallelVecProjectionGeometry2D::initialize(const Config& _cfg)
{
	ASTRA_ASSERT(_cfg.self);
	ConfigStackCheck<CProjectionGeometry2D> CC("ParallelVecProjectionGeometry2D", this, _cfg);	

	// TODO: Fix up class hierarchy... this class doesn't fit very well.
	// initialization of parent class
	//CProjectionGeometry2D::initialize(_cfg);

	// Required: DetectorCount
	XMLNode node = _cfg.self.getSingleNode("DetectorCount");
	ASTRA_CONFIG_CHECK(node, "ParallelVecProjectionGeometry2D", "No DetectorRowCount tag specified.");
	m_iDetectorCount = boost::lexical_cast<int>(node.getContent());
	CC.markNodeParsed("DetectorCount");

	// Required: Vectors
	node = _cfg.self.getSingleNode("Vectors");
	ASTRA_CONFIG_CHECK(node, "ParallelVecProjectionGeometry2D", "No Vectors tag specified.");
	vector<float32> data = node.getContentNumericalArray();
	CC.markNodeParsed("Vectors");
	ASTRA_CONFIG_CHECK(data.size() % 6 == 0, "ParallelVecProjectionGeometry2D", "Vectors doesn't consist of 6-tuples.");
	m_iProjectionAngleCount = data.size() / 6;
	m_pProjectionAngles = new SParProjection[m_iProjectionAngleCount];

	for (int i = 0; i < m_iProjectionAngleCount; ++i) {
		SParProjection& p = m_pProjectionAngles[i];
		p.fRayX  = data[6*i +  0];
		p.fRayY  = data[6*i +  1];
		p.fDetUX = data[6*i +  4];
		p.fDetUY = data[6*i +  5];

		// The backend code currently expects the corner of the detector, while
		// the matlab interface supplies the center
		p.fDetSX = data[6*i +  2] - 0.5f * m_iDetectorCount * p.fDetUX;
		p.fDetSY = data[6*i +  3] - 0.5f * m_iDetectorCount * p.fDetUY;
	}

	// success
	m_bInitialized = _check();
	return m_bInitialized;
}

//----------------------------------------------------------------------------------------
// Clone
CProjectionGeometry2D* CParallelVecProjectionGeometry2D::clone()
{
	return new CParallelVecProjectionGeometry2D(*this);
}

//----------------------------------------------------------------------------------------
// is equal
bool CParallelVecProjectionGeometry2D::isEqual(CProjectionGeometry2D* _pGeom2) const
{
	if (_pGeom2 == NULL) return false;

	// try to cast argument to CParallelVecProjectionGeometry2D
	CParallelVecProjectionGeometry2D* pGeom2 = dynamic_cast<CParallelVecProjectionGeometry2D*>(_pGeom2);
	if (pGeom2 == NULL) return false;

	// both objects must be initialized
	if (!m_bInitialized || !pGeom2->m_bInitialized) return false;

	// check all values
	if (m_iProjectionAngleCount != pGeom2->m_iProjectionAngleCount) return false;
	if (m_iDetectorCount != pGeom2->m_iDetectorCount) return false;
	
	for (int i = 0; i < m_iProjectionAngleCount; ++i) {
		if (memcmp(&m_pProjectionAngles[i], &pGeom2->m_pProjectionAngles[i], sizeof(m_pProjectionAngles[i])) != 0) return false;
	}

	return true;
}

//----------------------------------------------------------------------------------------
// Is of type
bool CParallelVecProjectionGeometry2D::isOfType(const std::string& _sType)
{
	return (_sType == "parallel_vec");
}

//----------------------------------------------------------------------------------------

CVector3D CParallelVecProjectionGeometry2D::getProjectionDirection(int _iProjectionIndex, int _iDetectorIndex /* = 0 */)
{
	CVector3D vOutput(0.0f, 0.0f, 0.0f);

	// not implemented
	ASTRA_ASSERT(false);

	return vOutput;
}

//----------------------------------------------------------------------------------------

void CParallelVecProjectionGeometry2D::getRayParams(int _iRow, int _iColumn, float32& _fT, float32& _fTheta) const
{
	// not implemented
	ASTRA_ASSERT(false);
}

//----------------------------------------------------------------------------------------

bool CParallelVecProjectionGeometry2D::_check()
{
	// TODO
	return true;
}


//----------------------------------------------------------------------------------------
// Get the configuration object
Config* CParallelVecProjectionGeometry2D::getConfiguration() const 
{
	Config* cfg = new Config();
	cfg->initialize("ProjectionGeometry2D");
	cfg->self.addAttribute("type", "parallel_vec");
	cfg->self.addChildNode("DetectorCount", getDetectorCount());
	std::string vectors = "";
	for (int i = 0; i < m_iProjectionAngleCount; ++i) {
		SParProjection& p = m_pProjectionAngles[i];
		vectors += boost::lexical_cast<string>(p.fRayX) + ",";
		vectors += boost::lexical_cast<string>(p.fRayY) + ",";
		vectors += boost::lexical_cast<string>(p.fDetSX + 0.5f * m_iDetectorCount * p.fDetUX) + ",";
		vectors += boost::lexical_cast<string>(p.fDetSY + 0.5f * m_iDetectorCount * p.fDetUY) + ",";
		vectors += boost::lexical_cast<string>(p.fDetUX) + ",";
		vectors += boost::lexical_cast<string>(p.fDetUY);
		if (i < m_iProjectionAngleCount-1) vectors += ';';
	}
	cfg->self.addChildNode("Vectors", vectors);
	return cfg;
}
//----------------------------------------------------------------------------------------


} // namespace astra
