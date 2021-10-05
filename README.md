# Service-wrapper

ROS provides easy ways to record bags from all the topics that are passed around in a ROS based software.
This is extremely useful for logging data and debugging. However, service calls do not receive this attention in ROS framework and there is no clear way of logging service calls that happen in a distributed ROS based software.

One way to log the services is to create topics for each server and client and create a wrapper that publishes the content of the service call whenever the service is used. I am not certain if this is really a good approach, but works. It is definitely the optimal solution.
