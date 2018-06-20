/**
 * File: FeatureVector.cpp
 * Date: November 2011
 * Author: Dorian Galvez-Lopez
 * Description: feature vector
 * License: see the LICENSE.txt file
 *
 */

#include "FeatureVector.h"
#include <map>
#include <vector>
#include <iostream>

namespace DBoW3 {

// ---------------------------------------------------------------------------

FeatureVector::FeatureVector(void)
{
}

// ---------------------------------------------------------------------------

FeatureVector::~FeatureVector(void)
{
}

// ---------------------------------------------------------------------------

void FeatureVector::addFeature(NodeId id, unsigned int i_feature)
{
  FeatureVector::iterator vit = this->lower_bound(id);
  
  if(vit != this->end() && vit->first == id)
  {
    vit->second.push_back(i_feature);
  }
  else
  {
    vit = this->insert(vit, FeatureVector::value_type(id, 
      std::vector<unsigned int>() ));
    vit->second.push_back(i_feature);
  }
}

// ---------------------------------------------------------------------------

std::ostream& operator<<(std::ostream &out, 
  const FeatureVector &v)
{
  if(!v.empty())
  {
    FeatureVector::const_iterator vit = v.begin();
    
    const std::vector<unsigned int>* f = &vit->second;

    out << "<" << vit->first << ": [";
    if(!f->empty()) out << (*f)[0];
    for(unsigned int i = 1; i < f->size(); ++i)
    {
      out << ", " << (*f)[i];
    }
    out << "]>";
    
    for(++vit; vit != v.end(); ++vit)
    {
      f = &vit->second;
      
      out << ", <" << vit->first << ": [";
      if(!f->empty()) out << (*f)[0];
      for(unsigned int i = 1; i < f->size(); ++i)
      {
        out << ", " << (*f)[i];
      }
      out << "]>";
    }
  }
  
  return out;  
}

//serialization
void FeatureVector::toStream(std::ostream &str) const
{
  uint32_t size = this->size();
  str.write((char*)&size,sizeof(size));
  for(auto drit = this->begin(); drit != this->end(); ++drit)
  {
    NodeId nid = drit->first;
    const std::vector<unsigned int>& features = drit->second;
    uint32_t fsize = features.size();

    // save info of last_nid
    str.write((char*)&nid,sizeof(nid));
    str.write((char*)&fsize,sizeof(fsize));
    
    for(auto i = features.begin(); i != features.end(); ++i)
    {
      const unsigned int &fid = *i;
      str.write((char*)&fid,sizeof(fid));
    }
  }
}
void FeatureVector::fromStream(std::istream &str)
{
  this->clear();
  uint32_t size;
  str.read((char*)&size,sizeof(size));

  FeatureVector::iterator dit;
  for(size_t i = 0; i < size; i++)
  {
    NodeId nid;
    uint32_t fsize;
    str.read((char*)&nid,sizeof(nid));
    str.read((char*)&fsize,sizeof(fsize));

    dit = this->insert(this->end(),
          make_pair(nid, std::vector<unsigned int>() ));

    dit->second.reserve(fsize);
    for(uint32_t j=0; j<fsize; j++)
    {
      unsigned int fid;
      str.read((char*)&fid,sizeof(fid));
      dit->second.push_back(fid);
    }
  }
}

// ---------------------------------------------------------------------------

} // namespace DBoW3
