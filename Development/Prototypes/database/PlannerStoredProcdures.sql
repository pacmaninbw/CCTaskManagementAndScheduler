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

