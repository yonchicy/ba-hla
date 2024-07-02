#ifndef __TRANSPORT_TASK_FED_AMB_H__
#define __TRANSPORT_TASK_FED_AMB_H__

#include <RTI/NullFederateAmbassador.h>

using namespace rti1516;

class TransportTaskFedAmb : public NullFederateAmbassador
{
	public:
		// variables //
		double federateTime;
		double federateLookahead;
	
		bool isRegulating;
		bool isConstrained;
		bool isAdvancing;
		bool isAnnounced;
		bool isReadyToRun;

		// methods //
        //! Constructor
		TransportTaskFedAmb();
        //! Destructor
		virtual ~TransportTaskFedAmb() throw();
		
		///////////////////////////////////
		// synchronization point methods //
		///////////////////////////////////
		virtual void synchronizationPointRegistrationSucceeded( std::wstring const &label )
			throw( FederateInternalError );

		virtual void synchronizationPointRegistrationFailed( std::wstring const &label,  SynchronizationFailureReason reason )
			throw( FederateInternalError );

		virtual void announceSynchronizationPoint( std::wstring const &label, VariableLengthData const &theUserSuppliedTag )
			throw( FederateInternalError );

		virtual void federationSynchronized( std::wstring const &label )
			throw( FederateInternalError );
	
		//////////////////////////
		// time related methods //
		//////////////////////////
		virtual void timeRegulationEnabled(LogicalTime const &theFederateTime) 
			throw ( InvalidLogicalTime, 
					NoRequestToEnableTimeRegulationWasPending, 
					FederateInternalError);

		virtual void timeConstrainedEnabled( LogicalTime const &theFederateTime ) 
			throw ( InvalidLogicalTime, 
					NoRequestToEnableTimeConstrainedWasPending, 
					FederateInternalError);

		virtual void timeAdvanceGrant( LogicalTime const &theTime) 
			throw ( InvalidLogicalTime, 
					JoinedFederateIsNotInTimeAdvancingState, 
					FederateInternalError);
		
		///////////////////////////////
		// object management methods //
		///////////////////////////////
		virtual void discoverObjectInstance ( ObjectInstanceHandle theObject, 
											  ObjectClassHandle theObjectClass, 
											  std::wstring const &theObjectInstanceName) 
			throw ( CouldNotDiscover, 
					ObjectClassNotKnown, 
					FederateInternalError);

		virtual void reflectAttributeValues( ObjectInstanceHandle theObject, 
											 AttributeHandleValueMap const &theAttributeValues, 
											 VariableLengthData const &theUserSuppliedTag, 
											 OrderType sentOrder, 
											 TransportationType theType) 
			throw ( ObjectInstanceNotKnown, 
					AttributeNotRecognized, 
					AttributeNotSubscribed, 
					FederateInternalError);

		virtual void reflectAttributeValues( ObjectInstanceHandle theObject, 
											 AttributeHandleValueMap const &theAttributeValues, 
											 VariableLengthData const &theUserSuppliedTag, 
											 OrderType sentOrder, 
											 TransportationType theType, 
											 RegionHandleSet const &theSentRegionHandleSet) 
			throw ( ObjectInstanceNotKnown, 
					AttributeNotRecognized, 
					AttributeNotSubscribed, 
					FederateInternalError);

		virtual void receiveInteraction (InteractionClassHandle theInteraction, ParameterHandleValueMap const &theParameterValues, VariableLengthData const &theUserSuppliedTag, OrderType sentOrder, TransportationType theType) throw (InteractionClassNotRecognized, InteractionParameterNotRecognized, InteractionClassNotSubscribed, FederateInternalError);
		virtual void receiveInteraction (InteractionClassHandle theInteraction, ParameterHandleValueMap const &theParameterValues, VariableLengthData const &theUserSuppliedTag, OrderType sentOrder, TransportationType theType, RegionHandleSet const &theSentRegionHandleSet) throw (InteractionClassNotRecognized, InteractionParameterNotRecognized, InteractionClassNotSubscribed, FederateInternalError);
		virtual void receiveInteraction (InteractionClassHandle theInteraction, ParameterHandleValueMap const &theParameterValues, VariableLengthData const &theUserSuppliedTag, OrderType sentOrder, TransportationType theType, LogicalTime const &theTime, OrderType receivedOrder) throw (InteractionClassNotRecognized, InteractionParameterNotRecognized, InteractionClassNotSubscribed, FederateInternalError);
		virtual void receiveInteraction (InteractionClassHandle theInteraction, ParameterHandleValueMap const &theParameterValues, VariableLengthData const &theUserSuppliedTag, OrderType sentOrder, TransportationType theType, LogicalTime const &theTime, OrderType receivedOrder, RegionHandleSet const &theSentRegionHandleSet) throw (InteractionClassNotRecognized, InteractionParameterNotRecognized, InteractionClassNotSubscribed, FederateInternalError);
		virtual void receiveInteraction (InteractionClassHandle theInteraction, ParameterHandleValueMap const &theParameterValues, VariableLengthData const &theUserSuppliedTag, OrderType sentOrder, TransportationType theType, LogicalTime const &theTime, OrderType receivedOrder, MessageRetractionHandle theHandle) throw (InteractionClassNotRecognized, InteractionParameterNotRecognized, InteractionClassNotSubscribed, InvalidLogicalTime, FederateInternalError);
		virtual void receiveInteraction (InteractionClassHandle theInteraction, ParameterHandleValueMap const &theParameterValues, VariableLengthData const &theUserSuppliedTag, OrderType sentOrder, TransportationType theType, LogicalTime const &theTime, OrderType receivedOrder, MessageRetractionHandle theHandle, RegionHandleSet const &theSentRegionHandleSet) throw (InteractionClassNotRecognized, InteractionParameterNotRecognized, InteractionClassNotSubscribed, InvalidLogicalTime, FederateInternalError);

		virtual void removeObjectInstance( ObjectInstanceHandle theObject, VariableLengthData const &theUserSuppliedTag, OrderType sentOrder) throw (ObjectInstanceNotKnown, FederateInternalError);
		

		virtual void removeObjectInstance( ObjectInstanceHandle theObject, VariableLengthData const &theUserSuppliedTag, OrderType sentOrder, LogicalTime const &theTime, OrderType receivedOrder) throw (ObjectInstanceNotKnown, FederateInternalError);
		virtual void removeObjectInstance (ObjectInstanceHandle theObject, VariableLengthData const &theUserSuppliedTag, OrderType sentOrder, LogicalTime const &theTime, OrderType receivedOrder, MessageRetractionHandle theHandle) throw (ObjectInstanceNotKnown, InvalidLogicalTime, FederateInternalError);
	
		
	/////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////// Private Section ////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////
	private:
		double convertTime( LogicalTime const &theTime );
        std::wstring variableLengthDataToWstring(const
        rti1516::VariableLengthData& variableLengthData);

};

#endif /*__TRANSPORT_TASK_FED_AMB_H__*/
