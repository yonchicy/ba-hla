
#ifndef _TRANSPORT_TASK_FEDERATE_H_
#define _TRANSPORT_TASK_FEDERATE_H_

#include "TransportTaskFedAmb.h"
#include <RTI/RTIambassador.h>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <vector>

#define READY_TO_RUN "ReadyToRun"

using namespace rti1516;

/**
 * Example Federate implementation
 */

class TransportTask;
class TransportTaskFederate
{
	public:
		std::unique_ptr<rti1516::RTIambassador> rtiamb; /*!< RTI ambassador to comunicate with RTI */
		TransportTaskFedAmb      *fedamb; /*!< Own Federate Ambassador implementation */

		// public methods //
        //! Constructor
		TransportTaskFederate();
        //! Destructor
		virtual ~TransportTaskFederate();
        //! Main Simulation Method
		void runFederate( std::string federateName, 
                          std::string fom, 
                          std::string address );

    double timeStep;
    unsigned long long task_num;
    std::mutex mtx;
    std::condition_variable cv;


    std::vector<std::thread> workers;
    void startTransportTask();
    TransportTask getTask();


	private:
		/// fom handles
		ObjectClassHandle      aHandle;
		AttributeHandle        aaHandle;
		AttributeHandle        abHandle;
		AttributeHandle        acHandle;
		InteractionClassHandle xHandle;
		ParameterHandle        xaHandle;
		ParameterHandle        xbHandle;

        std::wstring convertStringToWstring(const std::string& str);
        rti1516::VariableLengthData toVariableLengthData(const char* s);

		void initializeHandles();
		void waitForUser();
		void enableTimePolicy();
		void publishAndSubscribe();
		ObjectInstanceHandle registerObject();
		void updateAttributeValues( ObjectInstanceHandle objectHandle );
		void sendInteraction();
		void advanceTime( double timestep );
		void deleteObject( ObjectInstanceHandle objectHandle );
		double getLbts();
};

#endif /* _TRANSPORT_TASK_FEDERATE_H_ */
