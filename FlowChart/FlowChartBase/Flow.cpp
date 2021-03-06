// Framework codes
#include "flow.h"

static std::map<std::string, std::vector<std::shared_ptr<PinTypeBase> > > g_PinTypes;
static std::map<std::string, std::vector<std::shared_ptr<AlgthmBase>> > g_algs;

short RegisterPinType(std::shared_ptr<PinTypeBase> type)
{
	g_PinTypes[type->m_type].push_back(type);
	return 0;
}

short RegisterAlgthm(const char *name, std::shared_ptr<AlgthmBase> pAlg)
{
	pAlg->m_name = name;
	g_algs[name].push_back(pAlg);
	return 0;
}

std::shared_ptr<PinTypeBase> CreatePinInst(const char *name)
{
	auto itv = g_PinTypes.find(name);
	assert(itv != g_PinTypes.end());
	for (auto it = itv->second.begin(); it != itv->second.end(); ++it)
	{
		if (it->unique())
			return *it;
	}

	std::shared_ptr<PinTypeBase> t(itv->second[0]->Create());
	itv->second.push_back(t);
	return t;
}

std::shared_ptr<AlgthmBase> CrteateAlgInst(const char *name)
{
	auto itv = g_algs.find(name);
	assert(itv != g_algs.end());
	for (auto it = itv->second.begin(); it != itv->second.end(); ++it)
	{
		if (it->unique())
			return *it;
	}

	std::shared_ptr<AlgthmBase> t(itv->second[0]->Create());
	itv->second.push_back(t);
	return t;
}

class Flow::Impl
{
	std::vector< std::shared_ptr<AlgthmBase> > m_algs;

	struct Conn
	{
		std::pair<int, int> src;
		std::pair<int, int> dst;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(src);
			ar & BOOST_SERIALIZATION_NVP(dst);
		}
	};
	std::vector<Conn > m_conns;

	struct AlgSerialInfo
	{
		std::vector<AlgthmBase::Pin>    m_vInPins;
		std::vector<AlgthmBase::Pin>    m_vOutPins;
		std::string            m_name;

		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_NVP(m_name);
			ar & BOOST_SERIALIZATION_NVP(m_vInPins);
			ar & BOOST_SERIALIZATION_NVP(m_vOutPins);
		}
	};

	template <class T1, class T2>
	void MyCopyAlg(T1 const &t1, T2 &t2)
	{
		t2.m_name = t1.m_name;
		t2.m_vInPins = t1.m_vInPins;
		t2.m_vOutPins = t1.m_vOutPins;
	}
	// save
	template<class Archive>
	void serialize_m_algs(Archive & ar, const unsigned int version, boost::mpl::bool_<true>)
	{
		std::vector<AlgSerialInfo > vAlg(m_algs.size());
		for (size_t i = 0; i < m_algs.size(); i++)
		{
			MyCopyAlg(*m_algs[i], vAlg[i]);
		}
		ar & BOOST_SERIALIZATION_NVP(vAlg);
	}

	//load
	template<class Archive>
	void serialize_m_algs(Archive & ar, const unsigned int version, boost::mpl::bool_<false>)
	{
		std::vector<AlgSerialInfo > vAlg;
		ar & BOOST_SERIALIZATION_NVP(vAlg);

		for (size_t i = 0; i < vAlg.size(); i++)
		{
			int pid;
			AddModule(vAlg[i].m_name.c_str(), &pid, &vAlg[i].m_vInPins);
		}
	}
public:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		serialize_m_algs(ar, version, Archive::is_saving());
		ar & BOOST_SERIALIZATION_NVP(m_conns);
	}
private:
	short ConfigModule(int id, void *p)
	{
		assert(m_algs.size()>id);
		m_algs[id]->config(p);
	}


	struct RunItem{
		int aligId;	// curent algorithm idx
		std::vector<std::vector<std::pair<int, int> > > vtrigPin;	// next alg & pins that be triggerd by this alg
		std::vector<int> vIsInputValid;	// status of every input pins
		MyAlgEnv	m_algEnv;

		bool isInputValid() const
		{
			return (std::find(vIsInputValid.begin(), vIsInputValid.end(),0) == vIsInputValid.end());
		}
	};

	struct Env
	{
		std::vector<int> m_vGraph;	// Run sequence graph
		std::vector<int> m_preparedJobs;	// size is dynamically changing
		std::vector<RunItem> m_vRunItem;
		int m_startId;	//start and end item
		int m_endId;

		Env()
			:m_startId(-1), m_endId(-1)
		{}
	};

	short buildGraph(Env	&env)
	{
		size_t nAlg = m_algs.size();

		printf("\nbuilding graph, Num Alg[%d]\n", nAlg);
		env.m_vGraph.resize(nAlg*nAlg, -1);	// Run sequence graph
		env.m_vRunItem.resize(nAlg);
		
		// fill RunItem except vtrigPin[]
		for (size_t algId = 0; algId<nAlg; algId++)
		{
			RunItem &item = env.m_vRunItem[algId];
			AlgthmBase *alg = m_algs[algId].get();

			// algid
			item.aligId = algId;

			size_t nInput = alg->m_vInPins.size();
			size_t nOutput = alg->m_vOutPins.size();

			// vIsInputValid
			item.vIsInputValid.resize(nInput, 0);

			// vInPin
			item.m_algEnv.vInPin.resize(nInput);
			for (size_t i = 0; i < nInput; i++)
			{
				printf("alg %d InPin %d Type %s\n", algId,i, alg->m_vInPins[i].m_type.c_str());
				item.m_algEnv.vInPin[i] = CreatePinInst(alg->m_vInPins[i].m_type.c_str());
			}

			// vOutPin
			item.m_algEnv.vOutPin.resize(nOutput);
			for (size_t i = 0; i < nOutput; i++)
			{
				printf("alg %d OutPin %d Type %s\n", algId, i, alg->m_vOutPins[i].m_type.c_str());
				item.m_algEnv.vOutPin[i] = CreatePinInst(alg->m_vOutPins[i].m_type.c_str());
			}
		}

		// fill preparedJobs
		for (size_t algId = 0; algId < nAlg; algId++)
		{
			AlgthmBase *alg = m_algs[algId].get();
			if (alg->m_vInPins.size() == 0)
			{
				printf("m_preparedJobs added id %d\n", algId);
				env.m_preparedJobs.push_back(algId);
			}
		}

		printf("\n");
		// fill RunItem.vtrigPin, vGraph
		for (size_t i = 0; i < m_conns.size(); i++)
		{
			Conn &conn = m_conns[i];
			assert(conn.src.first <nAlg);
			assert(conn.dst.first <nAlg);
			AlgthmBase *algSrc = m_algs[conn.src.first].get();
			AlgthmBase *algDst = m_algs[conn.dst.first].get();
			assert(conn.src.second<algSrc->m_vOutPins.size());
			assert(conn.dst.second<algDst->m_vInPins.size());

			// fill vtrigPin
			RunItem &item = env.m_vRunItem[conn.src.first];
			item.vtrigPin.resize(algSrc->m_vOutPins.size());
			item.vtrigPin[conn.src.second].push_back(conn.dst);

			printf("Connection %d, %d %d->%d %d\n", i, conn.src.first, conn.src.second, conn.dst.first, conn.dst.second);

			// fill vGraph
#define G(x,y) env.m_vGraph[y*nAlg+x]
			G(conn.src.first, conn.dst.first) = 1;
		}
		printf("\n");

		return 0;
	}

	void setStartEndId(Env	&env, int startId, int endId)
	{
		env.m_startId = startId;
		env.m_endId = endId;
		// fill m_startId to m_preparedJobs 
		assert((env.m_startId >= 0 && env.m_endId >= 0) || (env.m_startId < 0 && env.m_endId < 0));
		if (env.m_startId >= 0 && env.m_endId >= 0)
		{
			printf("m_preparedJobs added start id %d\n", env.m_startId);
			env.m_preparedJobs.push_back(env.m_startId);
		}
	}
public:
	short AddModule(const char *name, int *pid, void *p=0)
	{
		std::shared_ptr<AlgthmBase> alg = CrteateAlgInst(name);

		m_algs.push_back(alg);
		*pid = m_algs.size() - 1;

		if (p)
			alg->config(p);
		
		return 0;
	}

	short ConnectModule(int mod1, int pinid1, int mod2, int pinid2)
	{
		assert(m_algs.size()>mod1);
		assert(m_algs.size()>mod2);

		AlgthmBase* alg1 = m_algs[mod1].get();
		AlgthmBase* alg2 = m_algs[mod2].get();
		assert(alg1->m_vOutPins.size()>pinid1);
		assert(alg2->m_vInPins.size()>pinid2);
		assert(alg1->m_vOutPins[pinid1].m_type == alg2->m_vInPins[pinid2].m_type);

		Conn conn;
		conn.src = std::make_pair(mod1, pinid1);
		conn.dst = std::make_pair(mod2, pinid2);
		m_conns.push_back(conn);
		return 0;
	}

	short run(
		std::vector<std::shared_ptr<PinTypeBase> >    *pvInPins = 0,
		std::vector<std::shared_ptr<PinTypeBase> >    *pvOutPins = 0,
		int startId = -1, int endId = -1	
		)
	{
		Env	env;
		buildGraph(env);

		if (env.m_vRunItem.empty())
		{
			assert(0);
			return -1;
		}

		//by default, the start & end are the 1st and last algorithm
		if (startId < 0)
			startId = 0;
		if (endId < 0)
			endId = env.m_vRunItem.size() - 1;

		if (startId >= env.m_vRunItem.size() || endId >= env.m_vRunItem.size())
		{
			assert(0);
			return -1;
		}
		
		env.m_startId = startId;
		env.m_endId = endId;
		// fill m_startId to m_preparedJobs 
		assert((env.m_startId >= 0 && env.m_endId >= 0) || (env.m_startId < 0 && env.m_endId < 0));
		
		// fill input pins of start 
		assert((env.m_startId >= 0 && env.m_endId >= 0) || (env.m_startId < 0 && env.m_endId < 0));
		if (env.m_startId >= 0 && env.m_endId >= 0 )
		{
			printf("fill input pins of start id %d\n", env.m_startId);
			auto& startItem =env.m_vRunItem[env.m_startId];
			
			if(pvInPins)
			{
				assert(pvInPins->size() == startItem.m_algEnv.vInPin.size());
				for (size_t i = 0; i < startItem.m_algEnv.vInPin.size(); i++)
				{
					printf("copy pin idx [%d] type %s\n", i, startItem.m_algEnv.vInPin[i]->m_type.c_str());
					startItem.m_algEnv.vInPin[i] = (*pvInPins)[i];
					
					startItem.vIsInputValid[i] = 1;
				}
			}
		
			assert(startItem.isInputValid());
		
			printf("m_preparedJobs added start id %d\n", env.m_startId);
			env.m_preparedJobs.push_back(env.m_startId);
		}

		while (1)
		{
			std::vector<int> nextPreparedJobs;
			for (size_t i = 0; i < env.m_preparedJobs.size(); i++)
			{
				int nAlgId = env.m_preparedJobs[i];
				auto& currItem =env.m_vRunItem[nAlgId];
				printf("===Run job [%d] algID %d...\n", i, nAlgId);
				
				// all input should be ready now
				assert(currItem.isInputValid());
				
				// run the task
				m_algs[nAlgId]->run(&currItem.m_algEnv);
				printf("Run job [%d] algID %d done\n", i, nAlgId);

				//post-processing
				//set vIsInputValid of next tasks
				printf("post-processing\n");
				for (size_t j = 0; j < currItem.vtrigPin.size(); j++)
				{
					auto vcurrTrigPort = currItem.vtrigPin[j];
					for (size_t k = 0; k < vcurrTrigPort.size(); k++)
					{
						auto next = vcurrTrigPort[k];
						auto& nextItem =env.m_vRunItem[next.first];
						auto& nextInpin =nextItem.m_algEnv.vInPin[next.second];
						// fill vIsInputValid
						nextItem.vIsInputValid[next.second] = 1;

						// fill data
						nextInpin = currItem.m_algEnv.vOutPin[j];
						
						printf("copy pin idx [%d] ->[%d %d] type %s\n", j, next.first, next.second, nextInpin->m_type.c_str());

						// if input of next task is ready, then add to nextPreparedJobs
						if (nextItem.isInputValid())
						{
							nextPreparedJobs.push_back(next.first);
							printf("add to nextPreparedJobs algid: %d\n", next.first);
						}
					}
				}
			}

			env.m_preparedJobs = nextPreparedJobs;
			if (nextPreparedJobs.empty())
				break;
		}

		// check endid's input is filled!!
		if (env.m_startId >= 0 && env.m_endId >= 0 && pvOutPins)
		{
			printf("check endid's input is filled!!\n");
			auto& endItem=env.m_vRunItem[env.m_endId];
			auto& endEnv = endItem.m_algEnv;
			pvOutPins->resize(endEnv.vOutPin.size());
			assert(pvOutPins->size() == endEnv.vOutPin.size());

			// all input should be ready now
			assert(endItem.isInputValid());

			for (size_t i = 0; i < endEnv.vOutPin.size(); i++)
			{
				(*pvOutPins)[i] = endEnv.vOutPin[i];
				printf("Copy out pin [%d]\n", i);
			}
		}

		return 0;
	}
	
	short io(const char *file, bool isSave)
	{
		Impl &flow = *this;
		if(isSave)
		{
			std::ofstream ofs(file);
			//boost::archive::text_oarchive oa(ofs);
			boost::archive::xml_oarchive oa(ofs);
			// write class instance to archive
			oa << BOOST_SERIALIZATION_NVP(flow);
			// archive and stream closed when destructors are called
		}
		else
		{
			std::ifstream ofs(file);
			//boost::archive::text_oarchive oa(ofs);
			boost::archive::xml_iarchive oa(ofs);
			// write class instance to archive
			oa >> BOOST_SERIALIZATION_NVP(flow);
			// archive and stream closed when destructors are called

		}
		return 0;
	}
	
	friend class Flow;
};

Flow::Flow()
:impl(new Impl)
{
}

Flow::~Flow()
{
delete impl;
}

short Flow::AddModule(const char *name, int *pid, void *p)
{
 return impl->AddModule( name, pid, p);
}

short Flow::ConnectModule(int mod1, int pinid1, int mod2, int pinid2)
{
 return impl->ConnectModule( mod1, pinid1, mod2, pinid2);
}

short Flow::run(std::vector<std::shared_ptr<PinTypeBase> >    *pvInPins,
	std::vector<std::shared_ptr<PinTypeBase> >    *pvOutPins,
	int startId , int endId )
{
	return impl->run(pvInPins, pvOutPins, startId, endId);
}

short Flow::io(const char *file, bool isSave)
{
	return impl->io(file, isSave);
}

void FreePool()
{
	g_PinTypes.clear();
	g_algs.clear();
}
