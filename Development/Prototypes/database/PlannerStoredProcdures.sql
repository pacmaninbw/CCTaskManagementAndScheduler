USE `PlannerTaskScheduleDB`;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `AddNewUserPreferences`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `AddNewUserPreferences`
(
	IN UserID INT UNSIGNED
)
BEGIN

	INSERT INTO PlatformIndepenentPreferences (
		PlatformIndepenentPreferences.UserID,
        PlatformIndepenentPreferences.UseLettersForPriorityLevel,
        PlatformIndepenentPreferences.SeparatePriorityWithDot,
        PlatformIndepenentPreferences.ScheduleDayStart,
        PlatformIndepenentPreferences.ScheduleDayEnd
    )
    
    VALUES (UserID, 1, 0, '8:30:00', '17:00:00');
    
END$$

DELIMITER ;


USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `AddNewUserLoginAndPassword`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `AddNewUserLoginAndPassword`
(
	IN UserID INT UNSIGNED,
    IN LoginName VARCHAR(45),
    IN HashedPassWord TINYTEXT
)
BEGIN

	INSERT INTO LoginAndPassword
		(
			LoginAndPassword.UserID,
            LoginAndPassword.LoginName,
            LoginAndPassword.HashedPassWord
		)
		VALUES (UserID, LoginName, HashedPassWord);
END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `AddNewUser`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `AddNewUser`
	(
        IN LastName VARCHAR(45),
		IN FirstName TINYTEXT,
		IN MiddleInitial TINYTEXT,
	    IN LoginName VARCHAR(45),
		IN HashedPassWord TINYTEXT
    )
BEGIN

	INSERT INTO UserProfile
		(
			UserProfile.LastName,
            UserProfile.FirstName,
            UserProfile.MiddleInitial
        )
		VALUES (LastName, FirstName, MiddleInitial);
        
        SET @NewUserID := LAST_INSERT_ID();
        
        CALL AddNewUserLoginAndPassword(@NewUserID, LoginName, HashedPassWord);
        
        CALL AddNewUserPreferences(@NewUserID);

END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `CreateTaskDates`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `CreateTaskDates`
(
	TaskID INT UNSIGNED,
    RequiredDelivery DATE,
    ScheduledStart DATE
)
BEGIN
	SET @CreationDate := curdate();
    
    INSERT INTO Dates
		(
			Dates.TaskID,
            Dates.CreatedOn,
            Dates.RequiredDelivery,
            Dates.ScheduledStart
		)
		VALUES (
			TaskID,
            @CreationDate,
            RequiredDelivery,
            ScheduledStart
		);
END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `CreateTaskEffort`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `CreateTaskEffort`
(
	TaskID INT UNSIGNED,
    EstimatedEffortHours INT UNSIGNED
)
BEGIN

	INSERT INTO Effort
		(
			Effort.TaskID,
            Effort.EstimatedEffortHours,
            Effort.ActualEffortHours
		)
        VALUES
        (
			TaskID,
            EstimatedEffortHours,
            0.0
        );
    
END$$

DELIMITER ;

USE `PlannerTaskScheduleDB`;
DROP procedure IF EXISTS `CreateTask`;

DELIMITER $$
USE `PlannerTaskScheduleDB`$$
CREATE PROCEDURE `CreateTask`
(
	IN CreatedBy INT UNSIGNED,
    IN Description TINYTEXT,
    IN ParentTask INT UNSIGNED,
    IN EstimatedEffortHours INT UNSIGNED,
    IN PriorityInAllTasks INT UNSIGNED,
    IN RequiredDelivery DATE,
    IN ScheduledStart DATE
)
BEGIN

	SET @TaskStatusValue = findTaskStatusValueByLabel('Not Started');
    
	INSERT INTO Tasks
		(
			Tasks.CreatedBy,
            Tasks.AsignedTo,
            Tasks.Description,
            Tasks.PriorityInAllTasks,
            Tasks.Status
        )
		VALUES
		(
			CreatedBy,
            CreatedBy,
            Description,
            PriorityInAllTasks,
            @TaskStatusValue
		);
        
		SET @NewTaskID := LAST_INSERT_ID();
        
        CALL CreateTaskDates(
			@NewTaskID,
            RequiredDelivery,
            ScheduledStart
		);
        
        CALL CreateTaskEffort
        (
			@NewTaskID,
            EstimatedEffortHours
        );

END$$

DELIMITER ;


