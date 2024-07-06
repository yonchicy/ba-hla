#include "TransportTaskFedAmb.h"
#include "TransportTaskFederate.h"
#include <RTI/RTI1516fedTime.h>
#include <iostream>

using namespace std;
using namespace rti1516;

/**
 * NOTE: The main part of the functions in this file is to print something on
 * stdout to see whether the function is called or not.
 */
TransportTaskFedAmb::TransportTaskFedAmb() {
  // initialize all the variable values
  this->federateTime = 0.0;
  this->federateLookahead = 1.0;

  this->isRegulating = false;
  this->isConstrained = false;
  this->isAdvancing = false;
  this->isAnnounced = false;
  this->isReadyToRun = false;
  this->federate = nullptr;
}
TransportTaskFedAmb::TransportTaskFedAmb(TransportTaskFederate *fed)
    : federate(fed) {
  this->federateTime = 0.0;
  this->federateLookahead = 1.0;

  this->isRegulating = false;
  this->isConstrained = false;
  this->isAdvancing = false;
  this->isAnnounced = false;
  this->isReadyToRun = false;
}

TransportTaskFedAmb::~TransportTaskFedAmb() throw() {}

/**
 * convert Logical System time to Federate time which can be understood by RTI
 */
double TransportTaskFedAmb::convertTime(LogicalTime const &theTime) {
  RTI1516fedTime castedTime = (RTI1516fedTime)theTime;
  return castedTime.getFedTime();
}

///////////////////////////////////////////////////////////////////////////////
/////////////////////// Synchronization Point Callbacks ///////////////////////
///////////////////////////////////////////////////////////////////////////////
void TransportTaskFedAmb::synchronizationPointRegistrationSucceeded(
    std::wstring const &label) throw(FederateInternalError) {
  wcout << L"Successfully registered sync point: " << label << endl;
}

void TransportTaskFedAmb::synchronizationPointRegistrationFailed(
    std::wstring const &label,
    SynchronizationFailureReason reason) throw(FederateInternalError) {
  wcout << L"Failed to register sync point: " << label << endl;
}

void TransportTaskFedAmb::announceSynchronizationPoint(
    std::wstring const &label,
    VariableLengthData const &theUserSuppliedTag) throw(FederateInternalError) {
  wcout << L"Synchronization point announced: " << label << endl;
  std::wstring compair = L"ReadyToRun";
  if (wcscmp(label.c_str(), compair.c_str()) == 0)
    this->isAnnounced = true;
}

void TransportTaskFedAmb::federationSynchronized(
    std::wstring const &label) throw(FederateInternalError) {
  wcout << L"Federation Synchronized: " << label << endl;
  std::wstring compair = L"ReadyToRun";
  if (wcscmp(label.c_str(), compair.c_str()) == 0)
    this->isReadyToRun = true;
}

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////// Time Callbacks ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////
void TransportTaskFedAmb::timeRegulationEnabled(
    LogicalTime const
        &theFederateTime) throw(InvalidLogicalTime,
                                NoRequestToEnableTimeRegulationWasPending,
                                FederateInternalError) {
  this->isRegulating = true;
  this->federateTime = convertTime(theFederateTime);
}

void TransportTaskFedAmb::timeConstrainedEnabled(
    LogicalTime const
        &theFederateTime) throw(InvalidLogicalTime,
                                NoRequestToEnableTimeConstrainedWasPending,
                                FederateInternalError) {
  this->isConstrained = true;
  this->federateTime = convertTime(theFederateTime);
}

void TransportTaskFedAmb::timeAdvanceGrant(LogicalTime const &theTime) throw(
    InvalidLogicalTime, JoinedFederateIsNotInTimeAdvancingState,
    FederateInternalError) {
  this->isAdvancing = false;
  this->federateTime = convertTime(theTime);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////// Object Management Callbacks /////////////////////////
///////////////////////////////////////////////////////////////////////////////

//                         //
// Discover Object Methods //
//                         //
void TransportTaskFedAmb::discoverObjectInstance(
    ObjectInstanceHandle theObject, ObjectClassHandle theObjectClass,
    std::wstring const &theObjectInstanceName) throw(CouldNotDiscover,
                                                     ObjectClassNotKnown,
                                                     FederateInternalError) {
  wcout << L"Discoverd Object: handle=" << theObject << L", classHandle="
        << theObjectClass << L", name=" << theObjectInstanceName << endl;
}

//                                 //
// Reflect Attribute Value Methods //
//                                 //
void TransportTaskFedAmb::reflectAttributeValues(
    ObjectInstanceHandle theObject,
    AttributeHandleValueMap const &theAttributeValues,
    VariableLengthData const &theUserSuppliedTag, OrderType sentOrder,
    TransportationType theType) throw(ObjectInstanceNotKnown,
                                      AttributeNotRecognized,
                                      AttributeNotSubscribed,
                                      FederateInternalError) {
  wcout << L"Reflection Received:" << endl;
}

void TransportTaskFedAmb::reflectAttributeValues(
    ObjectInstanceHandle theObject,
    AttributeHandleValueMap const &theAttributeValues,
    VariableLengthData const &theUserSuppliedTag, OrderType sentOrder,
    TransportationType theType,
    RegionHandleSet const
        &theSentRegionHandleSet) throw(ObjectInstanceNotKnown,
                                       AttributeNotRecognized,
                                       AttributeNotSubscribed,
                                       FederateInternalError) {
  wcout << L"Reflection Received:" << endl;
}

/**
 * convert variableLengthData to Wstring (deserialize)\n
 * for serialize function see ExampleCPPFederate class
 */
std::wstring TransportTaskFedAmb::variableLengthDataToWstring(
    const rti1516::VariableLengthData &variableLengthData) {
  if (!variableLengthData.size())
    return std::wstring();
  return std::wstring((const wchar_t *)variableLengthData.data(),
                      variableLengthData.size() /
                          sizeof(std::wstring::value_type));
}

/**
 * receiveInteraction with some detailed output
 */
void TransportTaskFedAmb::receiveInteraction(
    InteractionClassHandle theInteraction,
    ParameterHandleValueMap const &theParameterValues,
    VariableLengthData const &theUserSuppliedTag, OrderType sentOrder,
    TransportationType theType) throw(InteractionClassNotRecognized,
                                      InteractionParameterNotRecognized,
                                      InteractionClassNotSubscribed,
                                      FederateInternalError) {

  this->federate->startTransportTask();
}

void TransportTaskFedAmb::receiveInteraction(
    InteractionClassHandle theInteraction,
    ParameterHandleValueMap const &theParameterValues,
    VariableLengthData const &theUserSuppliedTag, OrderType sentOrder,
    TransportationType theType,
    RegionHandleSet const
        &theSentRegionHandleSet) throw(InteractionClassNotRecognized,
                                       InteractionParameterNotRecognized,
                                       InteractionClassNotSubscribed,
                                       FederateInternalError) {
  wcout << L"Interaction Received:" << endl;
}
void TransportTaskFedAmb::receiveInteraction(
    InteractionClassHandle theInteraction,
    ParameterHandleValueMap const &theParameterValues,
    VariableLengthData const &theUserSuppliedTag, OrderType sentOrder,
    TransportationType theType, LogicalTime const &theTime,
    OrderType receivedOrder) throw(InteractionClassNotRecognized,
                                   InteractionParameterNotRecognized,
                                   InteractionClassNotSubscribed,
                                   FederateInternalError) {
  wcout << L"Interaction Received:" << endl;
}
void TransportTaskFedAmb::receiveInteraction(
    InteractionClassHandle theInteraction,
    ParameterHandleValueMap const &theParameterValues,
    VariableLengthData const &theUserSuppliedTag, OrderType sentOrder,
    TransportationType theType, LogicalTime const &theTime,
    OrderType receivedOrder,
    RegionHandleSet const
        &theSentRegionHandleSet) throw(InteractionClassNotRecognized,
                                       InteractionParameterNotRecognized,
                                       InteractionClassNotSubscribed,
                                       FederateInternalError) {
  wcout << L"Interaction Received:" << endl;
}

void TransportTaskFedAmb::receiveInteraction(
    InteractionClassHandle theInteraction,
    ParameterHandleValueMap const &theParameterValues,
    VariableLengthData const &theUserSuppliedTag, OrderType sentOrder,
    TransportationType theType, LogicalTime const &theTime,
    OrderType receivedOrder,
    MessageRetractionHandle theHandle) throw(InteractionClassNotRecognized,
                                             InteractionParameterNotRecognized,
                                             InteractionClassNotSubscribed,
                                             InvalidLogicalTime,
                                             FederateInternalError) {
  wcout << L"Interaction Received:" << endl;
}

void TransportTaskFedAmb::receiveInteraction(
    InteractionClassHandle theInteraction,
    ParameterHandleValueMap const &theParameterValues,
    VariableLengthData const &theUserSuppliedTag, OrderType sentOrder,
    TransportationType theType, LogicalTime const &theTime,
    OrderType receivedOrder, MessageRetractionHandle theHandle,
    RegionHandleSet const
        &theSentRegionHandleSet) throw(InteractionClassNotRecognized,
                                       InteractionParameterNotRecognized,
                                       InteractionClassNotSubscribed,
                                       InvalidLogicalTime,
                                       FederateInternalError) {
  wcout << L"Interaction Received:" << endl;
}

//                       //
// Remove Object Methods //
//                       //
void TransportTaskFedAmb::removeObjectInstance(
    ObjectInstanceHandle theObject,
    VariableLengthData const &theUserSuppliedTag,
    OrderType sentOrder) throw(ObjectInstanceNotKnown, FederateInternalError) {
  wcout << L"Object Removed: handle=" << theObject << endl;
}

void TransportTaskFedAmb::removeObjectInstance(
    ObjectInstanceHandle theObject,
    VariableLengthData const &theUserSuppliedTag, OrderType sentOrder,
    LogicalTime const &theTime,
    OrderType receivedOrder) throw(ObjectInstanceNotKnown,
                                   FederateInternalError) {
  wcout << L"Object Removed: handle=" << theObject << endl;
}

void TransportTaskFedAmb::removeObjectInstance(
    ObjectInstanceHandle theObject,
    VariableLengthData const &theUserSuppliedTag, OrderType sentOrder,
    LogicalTime const &theTime, OrderType receivedOrder,
    MessageRetractionHandle theHandle) throw(ObjectInstanceNotKnown,
                                             InvalidLogicalTime,
                                             FederateInternalError) {
  wcout << L"Object Removed: handle=" << theObject << endl;
}
