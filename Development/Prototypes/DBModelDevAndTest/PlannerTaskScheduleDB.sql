SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;

DROP DATABASE IF EXISTS test_ptsdb;
CREATE DATABASE test_ptsdb;

-- --------------------------------------------------------

CREATE OR REPLACE TABLE test_ptsdb.organization_profile (
    id_organization INT UNSIGNED NOT NULL AUTO_INCREMENT,
    organization_name VARCHAR(256) NOT NULL,
    email_address VARCHAR(256) NOT NULL,
    phone_number VARCHAR(32) NOT NULL,
    primary_contact_user INT UNSIGNED,    # This should  be a foriegn key from user_profile, but that table does not exist yet
    secondary_contact_user INT UNSIGNED,  # This should  be a foriegn key from user_profile, but that table does not exist yet
    address_line_1 VARCHAR(256),
    address_line_2 VARCHAR(256),
    city VARCHAR(128),
    postal_code VARCHAR(32),
    state_or_province VARCHAR(256),
    nation VARCHAR(64),
    created_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (id_organization),
    INDEX organization_name_idx (organization_name ASC),
    INDEX fk_orgn_prime_contact_idx (primary_contact_user ASC)
);


-- --------------------------------------------------------

CREATE OR REPLACE TABLE test_ptsdb.user_profile (
    user_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    id_organization INT UNSIGNED,
    last_name VARCHAR(45) NOT NULL,
    first_name VARCHAR(45) NOT NULL,
    middle_initial VARCHAR(45),
    email_address VARCHAR(256),
    user_login VARCHAR(45) NOT NULL,
    hashed_password VARCHAR(45) NOT NULL,
    preferences MEDIUMTEXT NOT NULL,
    created_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    last_login DATETIME,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (user_id),
    INDEX full_name_idx (last_name, first_name, middle_initial),
    UNIQUE INDEX user_login_unique (user_login ASC),
    UNIQUE INDEX email_unique (email_address ASC),
    INDEX up_last_login (last_login DESC),
    CONSTRAINT fk_user_profile_org_id
        FOREIGN KEY (id_organization)
        REFERENCES organization_profile (id_organization)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    INDEX fk_user_orgid_idx (id_organization)
);

-- --------------------------------------------------------
-- Stored Procedures for user_profile
-- --------------------------------------------------------

USE test_ptsdb;
DELIMITER $$
CREATE OR REPLACE PROCEDURE test_ptsdb.hide_user
(
    IN user_id INT 
)

BEGIN

    UPDATE user_profile SET user_profile.deleted = 1 WHERE organization_profile.user_id = user_id;

END$$

CREATE OR REPLACE PROCEDURE add_user
(
    IN id_organization INT UNSIGNED,
    IN last_name VARCHAR(45),
    IN first_name VARCHAR(45),
    IN middle_initial VARCHAR(45),
    IN email_address VARCHAR(256),
    IN user_login VARCHAR(45),
    IN hashed_password VARCHAR(45),
    IN preferences MEDIUMTEXT
)
BEGIN

    INSERT INTO user_profile
        (
            id_organization, last_name, first_name, middle_initial, email_address, user_login, hashed_password, preferences, deleted
        )
        VALUES
        (
            id_organization,
            last_name,
            first_name,
            middle_initial,
            email_address,
            user_login,
            hashed_password,
            preferences,
            0
        )
        RETURNING user_id;

END$$

CREATE OR REPLACE PROCEDURE update_user_all_fields
(
    IN user_id INT UNSIGNED,
    IN id_organization INT UNSIGNED,
    IN last_name VARCHAR(45),
    IN first_name VARCHAR(45),
    IN middle_initial VARCHAR(45),
    IN email_address VARCHAR(256),
    IN user_login VARCHAR(45),
    IN hashed_password VARCHAR(45),
    IN preferences MEDIUMTEXT,
    IN last_login DATETIME
)

BEGIN

    UPDATE user_profile SET
        user_profile.id_organization = id_organization,
        user_profile.last_name = last_name,
        user_profile.first_name = first_name,
        user_profile.middle_initial = middle_initial,
        user_profile.email_address = email_address,
        user_profile.user_login = user_login,
        user_profile.hashed_password = hashed_password,
        user_profile.preferences = preferences,
        user_profile.last_login = last_login
    WHERE user_profile.user_id = user_id;

END$$

CREATE OR REPLACE PROCEDURE get_all_users
()

BEGIN

    SELECT * FROM user_profile;

END$$

CREATE OR REPLACE PROCEDURE get_user_by_id
(
    IN user_id INT UNSIGNED
)

BEGIN

    SELECT * FROM user_profile WHERE user_profile.user_id = user_id;

END$$

CREATE OR REPLACE PROCEDURE get_user_by_user_login
(
    IN user_login VARCHAR(45)
)

BEGIN

    SELECT * FROM user_profile WHERE user_profile.user_login = user_login;

END$$

CREATE OR REPLACE PROCEDURE get_user_by_email
(
    IN email_address VARCHAR(256)
)

BEGIN

    SELECT * FROM user_profile WHERE user_profile.email_address = email_address;

END$$

CREATE OR REPLACE PROCEDURE get_user_by_login_and_password
(
    IN user_login VARCHAR(45),
    IN hashed_password VARCHAR(45)
)

BEGIN

    SELECT * FROM user_profile WHERE user_profile.user_login = user_login  AND user_profile.hashed_password = hashed_password;

END$$

CREATE OR REPLACE PROCEDURE get_user_by_full_name
(
    IN last_name VARCHAR(45),
    IN first_name VARCHAR(45),
    IN middle_initial VARCHAR(45)
)

BEGIN

    SELECT * FROM user_profile
    WHERE user_profile.last_name = last_name
        AND user_profile.first_name = first_name
        AND user_profile.middle_initial = middle_initial;

END$$

DELIMITER ;


-- --------------------------------------------------------

CREATE OR REPLACE TABLE  test_ptsdb.user_goals (
    id_user_goals INT UNSIGNED NOT NULL AUTO_INCREMENT,
    user_id INT UNSIGNED NOT NULL,
    description VARCHAR(1024) NOT NULL,
    priority INT DEFAULT NULL,
    parent_goal INT UNSIGNED DEFAULT NULL,
    creation_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (id_user_goals, user_id),
    UNIQUE INDEX id_user_goals_unique (id_user_goals ASC),
    INDEX ug_description_idx (description ASC),
    INDEX ug_creation_timestamp_idx (creation_timestamp DESC),
    INDEX ug_last_modified_time_stamp_idx (last_modified_time_stamp DESC),
    CONSTRAINT `fk_user_goals_user_id`
        FOREIGN KEY (user_id)
        REFERENCES user_profile (user_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    INDEX fk_user_goals_user_id_idx (user_id)
);

-- --------------------------------------------------------
-- Stored Procedures for user_goals
-- --------------------------------------------------------

USE test_ptsdb;
DELIMITER $$
CREATE OR REPLACE PROCEDURE test_ptsdb.hide_goal
(
    IN user_id INT,
    IN id_user_goals INT
)

BEGIN

    UPDATE user_goals
        SET user_goals.deleted = 1
        WHERE user_goals.user_id = user_id AND user_goals.id_user_goals = id_user_goals;

END$$

CREATE OR REPLACE PROCEDURE add_user_goal
(
    IN user_id INT UNSIGNED,
    IN description VARCHAR(1024),
    IN priority INT ZEROFILL,
    IN parent_goal INT UNSIGNED ZEROFILL
)

BEGIN

    INSERT INTO user_goals
        (
            user_id,
            description,
            priority,
            parent_goal,
            deleted
        )
        VALUES
        (
            user_id,
            description,
            priority,
            parent_goal,
            0
        )
        RETURNING id_user_goals;

END$$

CREATE OR REPLACE PROCEDURE update_user_goal_all_fields
(
    IN user_id INT UNSIGNED,
    IN id_user_goals INT UNSIGNED,
    IN description VARCHAR(1024),
    IN priority INT UNSIGNED,
    IN parent_goal INT UNSIGNED
)
BEGIN

    UPDATE user_goals SET
        user_goals.description = description,
        user_goals.priority = priority,
        user_goals.parent_goal = parent_goal
    WHERE user_goals.user_id = user_id AND user_goals.id_user_goals = id_user_goals;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.get_goal_by_id
(
    IN id_user_goals INT UNSIGNED
)

BEGIN

    SELECT * FROM user_goals WHERE user_goals.id_user_goals = id_user_goals;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`FindGoalByUserIdAndExactdescription`
(
    IN user_id INT UNSIGNED,
    IN `fulldescription` VARCHAR(1024)
)

BEGIN

    SELECT * FROM user_goals
    WHERE user_goals.user_id = user_id
    AND user_goals.description = fulldescription
        AND user_goals.deleted <> 1;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetAllGoalsForUser`
(
    IN user_id INT UNSIGNED
)

BEGIN

    SELECT * FROM user_goals
    WHERE user_goals.user_id = user_id
        AND user_goals.deleted <> 1;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetAllChildGoalsFromParent`
(
    IN user_id INT UNSIGNED,
    IN `parent_goal` INT UNSIGNED
)

BEGIN

    SELECT * FROM user_goals
    WHERE user_goals.user_id = user_id
        AND user_goals.parent_goal = parent_goal
        AND user_goals.deleted <> 1;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`FindGoalsByUserIdAndSimilardescription`
(
    IN user_id INT UNSIGNED,
    IN `partialdescription` VARCHAR(1024)
)

BEGIN

    SELECT * FROM user_goals
    WHERE user_goals.user_id = user_id AND user_goals.description LIKE CONCAT('%', partialdescription, '%')
        AND user_goals.deleted <> 1;

END$$

DELIMITER ;

-- --------------------------------------------------------

CREATE OR REPLACE TABLE test_ptsdb.user_notes (
    id_user_notes INT UNSIGNED NOT NULL AUTO_INCREMENT,
    user_id INT UNSIGNED NOT NULL,
    note_creation TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    content VARCHAR(1024) NOT NULL,
    last_modifed TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (id_user_notes, user_id),
    INDEX note_creation (note_creation DESC),
    INDEX user_notes_last_modifed (last_modifed DESC),
    INDEX un_content_idx (content ASC),
    INDEX fk_user_notes_user_id_idx (user_id ASC),
    CONSTRAINT fk_user_notes_user_id
      FOREIGN KEY (user_id)
      REFERENCES test_ptsdb.user_profile (user_id)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
);
    
-- --------------------------------------------------------
-- Stored Procedures for user_notes
-- --------------------------------------------------------

USE test_ptsdb;

DELIMITER $$
CREATE OR REPLACE PROCEDURE test_ptsdb.`HideNote`
(
    IN user_id INT,
    IN NoteID INT
)

BEGIN

    UPDATE user_notes
        SET user_notes.deleted = 1
        WHERE user_notes.user_id = user_id AND user_notes.id_user_notes = NoteID;

END$$

CREATE OR REPLACE PROCEDURE `AddUserNote`
(
    IN user_id INT UNSIGNED,
    IN content VARCHAR(1024)
)

BEGIN

    INSERT INTO user_notes(user_id, content, deleted)
        VALUES (user_id, content, 0)
        RETURNING id_user_notes;

END$$

CREATE OR REPLACE PROCEDURE `UpdateNoteAllFields`
(
    IN user_id INT UNSIGNED,
    IN note_id INT UNSIGNED,
    IN content VARCHAR(1024)
)

BEGIN

    UPDATE user_notes SET
        user_notes.content = content
    WHERE user_notes.id_user_notes = note_id AND user_notes.user_id = user_id;
    
END$$

CREATE OR REPLACE PROCEDURE `GetNoteByID`
(
    IN `noteId` INT UNSIGNED
)

BEGIN
 
    SELECT * FROM user_notes  WHERE user_notes.id_user_notes = noteId;
 
END$$
 
CREATE OR REPLACE PROCEDURE `GetAllNotesForUser` (IN user_id INT UNSIGNED)   BEGIN
  
    SELECT * FROM user_notes WHERE user_notes.user_id = user_id;

END$$
 
CREATE OR REPLACE PROCEDURE `GetAllUndeletedNotesForUser`
(
    IN user_id INT UNSIGNED
)

BEGIN

    SELECT * FROM user_notes
    WHERE user_notes.user_id = user_id
        AND user_notes.deleted <> 1;
 
END$$

CREATE OR REPLACE PROCEDURE `GetNotesForUserSimlarTocontent`
(
    IN user_id INT UNSIGNED,
    IN `likecontent` VARCHAR(1024)
)
BEGIN
 
    SELECT * FROM user_notes 
    WHERE user_notes.user_id = user_id
        AND user_notes.content LIKE CONCAT('%', likecontent, '%')
        AND user_notes.deleted <> 1;
    
END$$    
 
CREATE OR REPLACE PROCEDURE `GetAllNotesForUserCreatedInDatgeRange`
(
    IN user_id INT UNSIGNED,
    IN `timePeriodStart` DATE,
    IN `timePeriodEnd` DATE
) 
BEGIN

    SELECT * FROM user_notes
    WHERE user_notes.user_id = 4
        AND user_notes.note_creation >= timePeriodStart
        AND user_notes.note_creation <= timePeriodEnd
        AND user_notes.deleted <> 1;
 
END$$
 
CREATE OR REPLACE PROCEDURE `GetAllNotesForUserEditedInDatgeRange`
(
    IN user_id INT UNSIGNED,
    IN `timePeriodStart` DATE,
    IN `timePeriodEnd` DATE
) 
BEGIN

    SELECT * FROM user_notes
    WHERE user_notes.user_id = 4
        AND user_notes.last_modifed >= timePeriodStart
        AND user_notes.last_modifed <= timePeriodEnd
        AND user_notes.deleted <> 1;
 
END$$
 
CREATE OR REPLACE PROCEDURE `GetDashboardNoteTable` 
(
    IN user_id INT UNSIGNED,
    IN `startDay` DATETIME,
    IN `endDay` DATETIME
)
BEGIN

    SELECT * FROM user_notes
    WHERE user_notes.user_id = user_id
        AND user_notes.note_creation >= startDay
        AND user_notes.note_creation <= endDay
        AND user_notes.deleted <> 1
    ORDER BY user_notes.note_creation ASC;

END$$

DELIMITER ;

-- --------------------------------------------------------

CREATE OR REPLACE TABLE test_ptsdb.tasks (
    task_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    created_by INT UNSIGNED NOT NULL,
    assigned_to INT UNSIGNED NOT NULL,
    description VARCHAR(256) NOT NULL,
    parent_task INT UNSIGNED DEFAULT NULL,
    task_status INT UNSIGNED DEFAULT NULL,
    creation_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    due_date DATE NOT NULL,
    planned_start DATE NOT NULL,
    actual_start DATE DEFAULT NULL,
    estimated_delivery DATE DEFAULT NULL,
    delivered DATE DEFAULT NULL,
    est_hours_effort DOUBLE NOT NULL,
    hours_effort DOUBLE NOT NULL,
    priority_category INT UNSIGNED NOT NULL,
    priority INT UNSIGNED NOT NULL,
    personal TINYINT DEFAULT 0,
    dependency_count INT UNSIGNED,
    dependencies MEDIUMTEXT,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (task_id, created_by),
    INDEX fk_tasks_created_by_idx (created_by ASC),
    INDEX fk_tasks_assigned_to_idx (assigned_to ASC),
    INDEX task_description_idx (description ASC),
    INDEX task_last_modified_time_stamp_idx (last_modified_time_stamp DESC),
    INDEX task_priority_category_idx (priority_category ASC),
    INDEX task_priority_idx (priority ASC),
    INDEX task_task_status_idx (task_status ASC),
    INDEX task_due_date_idx (due_date ASC),
    INDEX task_planned_start_idx (planned_start ASC),
    CONSTRAINT fk_tasks_created_by
        FOREIGN KEY (created_by)
        REFERENCES user_profile (user_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    CONSTRAINT fk_tasks_assigned_to
        FOREIGN KEY (assigned_to)
        REFERENCES user_profile (user_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------
-- Stored Procedures for tasks
-- --------------------------------------------------------

USE test_ptsdb;

DELIMITER $$
CREATE OR REPLACE PROCEDURE test_ptsdb.`HideTask`
(
    IN user_id INT,
    IN IDTask INT
)

BEGIN

    UPDATE tasks SET tasks.deleted = 1 WHERE tasks.created_by = user_id AND tasks.task_id = IDTask;

END$$

CREATE OR REPLACE PROCEDURE `AddTask`
(
    IN `creatorID` INT UNSIGNED,
    IN `assignedID` INT UNSIGNED,
    IN description VARCHAR(256),
    IN `parenttask_id` INT UNSIGNED,
    IN `tasktask_status` INT UNSIGNED,
    IN `dueDate` DATE,
    IN `planStart` DATE,
    IN `startDate` DATE,
    IN `expectedDate` DATE,
    IN `completedDate` DATE,
    IN `estimatedEffort` DOUBLE,
    IN `effortToDate` DOUBLE,
    IN `priorityCategory` INT UNSIGNED,
    IN priority INT UNSIGNED,
    IN `ispersonal` TINYINT,
    IN `dependencyCount` INT UNSIGNED,
    IN dependencies MEDIUMTEXT
)
BEGIN

    INSERT INTO tasks
    (
        created_by,
        assigned_to,
        description,
        parent_task,
        task_status,
        due_date,
        planned_start,
        actual_start,
        estimated_delivery,
        delivered,
        est_hours_effort,
        hours_effort,
        priority_category,
        priority,
        personal,
        dependency_count,
        dependencies,
        deleted
    )
    VALUES
    (
        creatorID,
        assignedID,
        description,
        parenttask_id,
        tasktask_status,
        dueDate,
        planStart,
        startDate,
        expectedDate,
        completedDate,
        estimatedEffort,
        effortToDate,
        priorityCategory,
        priority,
        ispersonal,
        dependencyCount,
        dependencies,
        0
    )
    RETURNING task_id;

END$$

CREATE OR REPLACE PROCEDURE `UpdateTaskAllFields`
(
    IN `primaryKeyValue` INT UNSIGNED,
    IN `creatorID` INT UNSIGNED,
    IN `assignedID` INT UNSIGNED,
    IN description VARCHAR(256),
    IN `parenttask_id` INT UNSIGNED,
    IN `tasktask_status` INT UNSIGNED,
    IN `dueDate` DATE,
    IN `planStart` DATE,
    IN `startDate` DATE,
    IN `completedDate` DATE,
    IN `estimatedEffort` DOUBLE,
    IN `effortToDate` DOUBLE,
    IN `priorityCategory` INT UNSIGNED,
    IN priority INT UNSIGNED,
    IN `ispersonal` TINYINT,
    IN `dependencyCount` INT UNSIGNED,
    IN dependencies MEDIUMTEXT,
    IN deleted TINYINT
)

BEGIN

    UPDATE tasks SET
        tasks.created_by = creatorID,
        tasks.assigned_to = assignedID,
        tasks.description = description,
        tasks.parent_task = parenttask_id,
        tasks.task_status = tasktask_status,
        tasks.due_date = dueDate,
        tasks.planned_start = planStart,
        tasks.actual_start = startDate,
        tasks.delivered = completedDate,
        tasks.est_hours_effort = estimatedEffort,
        tasks.hours_effort = effortToDate,
        tasks.priority_category = priorityCategory,
        tasks.priority = priority,
        tasks.personal = ispersonal,
        tasks.dependency_count = dependencyCount,
        tasks.dependencies = dependencies,
        tasks.deleted = deleted
    WHERE tasks.task_id = primaryKeyValue;
    
END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetTaskByTaskID`
(
    IN IDTask INT
)

BEGIN

    SELECT * FROM tasks WHERE tasks.task_id = IDTask;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetTaskBydescriptionAndAssignedUser`
(
    IN `assignedID` INT UNSIGNED,
    IN description VARCHAR(256)
)

BEGIN

    SELECT * FROM tasks WHERE tasks.description = description AND tasks.assigned_to = assignedID AND tasks.deleted <> 1;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetActivetasksForAssignedUser`
(
    IN `assignedID` INT UNSIGNED
)

BEGIN

    SELECT * FROM tasks
    WHERE tasks.assigned_to = assignedID
        AND tasks.delivered IS NULL
        AND (tasks.task_status IS NOT NULL AND tasks.task_status <> 0)
        AND tasks.deleted <> 1;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetUnstartedDueForStartForAssignedUser`
(
    IN `assignedID` INT UNSIGNED,
    IN `planStart` DATE
)

BEGIN

    SELECT * FROM tasks
    WHERE tasks.assigned_to = assignedID
        AND tasks.planned_start < planStart
        AND (tasks.task_status IS NULL OR tasks.task_status = 0)
        AND tasks.deleted <> 1;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetTasksCompletedByAssignedAfterDate`
(
    IN `assignedID` INT UNSIGNED,
    IN `searchStartDate` DATE
)

BEGIN

    SELECT * FROM tasks
    WHERE tasks.assigned_to = assignedID
        AND tasks.delivered >= searchStartDate;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetTasksByAssignedIDandParentID`
(
    IN `assignedID` INT UNSIGNED,
    IN `parentID` INT UNSIGNED
)

BEGIN

    SELECT * FROM tasks
    WHERE tasks.assigned_to = assignedID
        AND tasks.parent_task = parentID
        AND tasks.deleted <> 1;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetDefaultDashboardTaskList`
(
    IN `assignedID` INT UNSIGNED,
    IN `dueDate` DATE
)

BEGIN

    SELECT * FROM tasks
    WHERE tasks.assigned_to = assignedID
        AND tasks.delivered IS NULL
        AND tasks.deleted <> 1
        AND (tasks.task_status = 3 OR tasks.due_date < dueDate)
    ORDER BY tasks.priority_category ASC, tasks.priority ASC;

END$$

DELIMITER ;
-- --------------------------------------------------------
-- End of Task related stored procedures
-- --------------------------------------------------------

-- --------------------------------------------------------

CREATE OR REPLACE TABLE  test_ptsdb.user_task_goals (
    user_id INT UNSIGNED NOT NULL,
    task_id  INT UNSIGNED NOT NULL,
    task_goal_list VARCHAR(45) NOT NULL,
    creation_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (user_id, task_id),
    INDEX utg_task_idx (task_id ASC),
    INDEX utg_creation_timestamp_idx (creation_timestamp DESC),
    INDEX utg_last_modified_time_stamp_idx (last_modified_time_stamp DESC),
    CONSTRAINT fk_user_task_goals_assigned_to
        FOREIGN KEY (user_id)
        REFERENCES user_profile (user_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    CONSTRAINT fk_user_task_goals_task_id
        FOREIGN KEY (task_id)
        REFERENCES tasks (task_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------
-- Stored Procedures for user_task_goals
-- --------------------------------------------------------

USE test_ptsdb;

-- --------------------------------------------------------

CREATE OR REPLACE TABLE test_ptsdb.user_schedule_item (
    id_user_schedule_item INT UNSIGNED NOT NULL AUTO_INCREMENT,
    user_id INT UNSIGNED NOT NULL,
    start_date_time DATETIME NOT NULL,
    end_date_time DATETIME NOT NULL,
    title VARCHAR(128) NOT NULL,
    personal TINYINT NOT NULL,
    location VARCHAR(128) DEFAULT NULL,
    created_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (id_user_schedule_item, user_id),
    INDEX schedule_item_title_idx (title ASC),
    INDEX schedule_item_start_date_time_idx (start_date_time DESC),
    INDEX schedule_item_created_timestamp_idx (created_timestamp DESC),
    INDEX schedule_item_last_modified_time_stamp_idx (last_modified_time_stamp DESC),
    INDEX fk_user_schedule_item_user_id_idx (user_id ASC),
    INDEX schedule_item_location_idx (location ASC),
    CONSTRAINT fk_user_schedule_item_user_id
        FOREIGN KEY (user_id)
        REFERENCES test_ptsdb.user_profile (user_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

-- --------------------------------------------------------
-- Stored Procedures for user_schedule_item
-- --------------------------------------------------------

USE test_ptsdb;

DELIMITER $$
CREATE OR REPLACE PROCEDURE test_ptsdb.`ScheduleItemcontentSelectionList`
(
    IN content VARCHAR(128),
    IN user_id INT
)

BEGIN

    SELECT DISTINCT user_schedule_item.title FROM user_schedule_item
    WHERE
        user_schedule_item.user_id = user_id AND user_schedule_item.title LIKE CONCAT('%', content, '%')
        ORDER BY user_schedule_item.title ASC;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`EventtitlesForCompleter`
(
    IN user_id INT
)

BEGIN

    SELECT DISTINCT user_schedule_item.title FROM user_schedule_item
    WHERE
        user_schedule_item.user_id = user_id
        ORDER BY user_schedule_item.title ASC;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`EventlocationsForCompleter`
(
    IN user_id INT
)

BEGIN

    SELECT DISTINCT user_schedule_item.location FROM user_schedule_item
    WHERE
        user_schedule_item.user_id = user_id
        ORDER BY user_schedule_item.location ASC;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`HideScheduleItem`
(
    IN user_id INT,
    IN ScheduleItemID INT
)

BEGIN

    UPDATE user_schedule_item
        SET user_schedule_item.deleted = 1
        WHERE user_schedule_item.user_id = user_id AND user_schedule_item.id_user_schedule_item = ScheduleItemID;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetScheduleItemById`
(
    IN scheduleItemID INT UNSIGNED 
)

BEGIN

    SELECT * FROM user_schedule_item
    WHERE user_schedule_item.id_user_schedule_item = scheduleItemID
        AND user_schedule_item.deleted <> 1;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`GetUserDaySchedule`
(
    IN user_id INT UNSIGNED,
    IN `eventStart` DATETIME,
    IN `eventEnd` DATETIME
)

BEGIN

    SELECT * FROM user_schedule_item
    WHERE user_schedule_item.user_id = user_id
        AND user_schedule_item.deleted <> 1
        AND user_schedule_item.start_date_time >= eventStart
        AND user_schedule_item.start_date_time <= eventEnd
    ORDER BY user_schedule_item.start_date_time ASC;

END$$

CREATE OR REPLACE PROCEDURE test_ptsdb.`FindUserScheduleItemsByContentAndDateRange`
(
    IN user_id INT UNSIGNED,
    IN `matchcontent`VARCHAR(128),
    IN `searchStart` DATE,
    IN `searchEnd` DATE
)

BEGIN

    SELECT * FROM user_schedule_item  WHERE user_id = user_id
    AND title LIKE CONCAT('%', matchcontent, '%')
    AND deleted <> 1
    AND start_date_time >= searchStart
    AND start_date_time <= searchEnd;

END$$

CREATE OR REPLACE PROCEDURE `AddScheduleEvent`
(
    IN user_id INT UNSIGNED,
    IN `eventStart` DATETIME,
    IN `eventEnd` DATETIME,
    IN `eventtitle` VARCHAR(128),
    IN `ispersonal` TINYINT,
    IN location VARCHAR(128)
)

BEGIN

INSERT INTO user_schedule_item
    (
        user_id, start_date_time, end_date_time, title, personal, location, deleted
    )
    VALUES
    (
        user_id,
        eventStart,
        eventEnd,
        eventtitle,
        ispersonal,
        location,
        0
    )
    RETURNING id_user_schedule_item;
    
END$$

CREATE OR REPLACE PROCEDURE `UpdateScheduleItemAllFields`
(
    IN user_id INT UNSIGNED,
    IN `eventId` INT UNSIGNED,
    IN `startTime` DATETIME,
    IN `endTime` DATETIME,
    IN title VARCHAR(128),
    IN personal TINYINT,
    IN location VARCHAR(128)
)
BEGIN

    UPDATE user_schedule_item SET
        user_schedule_item.start_date_time = startTime,
        user_schedule_item.end_date_time = endTime,
        user_schedule_item.title = title,
        user_schedule_item.personal = personal,
        user_schedule_item.location = location
    WHERE user_schedule_item.id_user_schedule_item = eventId AND user_schedule_item.user_id = user_id;
    
END$$
DELIMITER ;

COMMIT;


