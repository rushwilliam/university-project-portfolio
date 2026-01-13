import sqlite3
import random

HISTORY_LENGTH = 5
WEIGHT_DECAY = 0.1
CORRECT_NO_HINT = 0
CORRECT_HINT = 1
WRONG_NO_HINT = 2
WRONG_HINT = 3
NOISE = 1

# Returns an integer value based on a question attempt
def attempt_value(is_correct, used_hint):
    if is_correct:
        if used_hint:
            return CORRECT_HINT
        return CORRECT_NO_HINT
    if used_hint:
        return WRONG_HINT
    return WRONG_NO_HINT

# Returns the current score for a specific question and user
# A question with no history will return the value of WRONG_HINT
def question_score(cursor, user_id, question_id):
    cursor.execute("""
        SELECT is_correct, used_hint
        FROM UserProgress
        WHERE user_id = ? AND question_id = ?
        ORDER BY progress_id DESC
        LIMIT ?
        """, (user_id, question_id, HISTORY_LENGTH,))
    result_history = cursor.fetchall()
    total_score = 0
    weight = 1
    for result in result_history:
        total_score += attempt_value(result[0], result[1]) * weight
        weight -= WEIGHT_DECAY
    length = len(result_history)
    difference = HISTORY_LENGTH - length
    if difference > 0:
        total_score += WRONG_HINT * difference * (1 - WEIGHT_DECAY * (2 * HISTORY_LENGTH - difference - 1) / 2)
    total_weights = HISTORY_LENGTH - (HISTORY_LENGTH * (HISTORY_LENGTH - 1) * WEIGHT_DECAY) / 2
    score = total_score / total_weights
    return score

# Returns the score for a topic, which is the average of all questions even if they have not been seen yet
def topic_score(cursor, user_id, topic_id):
    cursor.execute("""
        SELECT question_id
        FROM questions
        WHERE lesson_id = ?
        """, (topic_id,))
    question_ids = cursor.fetchall()
    if len(question_ids) == 0:
        return WRONG_HINT
    total_score = 0
    for question_id in question_ids:
        total_score += question_score(cursor, user_id, question_id[0])
    score = total_score / len(question_ids)
    return score

# Returns the lesson_id of the weakest topic
def weak_topic(cursor, user_id):
    cursor.execute("""
        SELECT DISTINCT lesson_id
        FROM questions
        """)
    topics = cursor.fetchall()
    scores = [(topic[0], topic_score(cursor, user_id, topic[0])) for topic in topics]
    weakest = max(scores, key=lambda x: x[1])[0]
    return weakest

# Returns the lesson_id of the strongest topic
def strong_topic(cursor, user_id):
    cursor.execute("""
        SELECT DISTINCT lesson_id
        FROM questions
        """)
    topics = cursor.fetchall()
    scores = [(topic[0], topic_score(cursor, user_id, topic[0])) for topic in topics]
    strongest = min(scores, key=lambda x: x[1])[0]
    return strongest

# Returns a list of question ids from a topic. Questions with worse scores are more likely to be selected
# Adjust the randomness value by adjust the NOISE value
def get_topic_questions(cursor, user_id, topic_id, number_of_questions):
    cursor.execute("""
        SELECT question_id
        FROM questions
        WHERE lesson_id = ?
        """, (topic_id,))
    question_ids = [id[0] for id in cursor.fetchall()]
    if number_of_questions > len(question_ids):
        print("More questions pulled than questions in the database")
        exit()
    elif number_of_questions == len(question_ids):
        return question_ids
    scores = [(question_id, question_score(cursor, user_id, question_id)) for question_id in question_ids]
    question_list = []
    for _ in range(number_of_questions):
        weights = [score + NOISE for (_, score) in scores]
        selected = random.choices(scores, weights=weights, k=1)[0]
        question_list.append(selected[0])
        scores.remove(selected)
    return question_list

# Returns the total number of hints used for a topic
def topic_hints_used(cursor, user_id, topic_id):
    cursor.execute("""
        SELECT UserProgress.question_id
        FROM UserProgress
        JOIN questions ON UserProgress.question_id = questions.question_id
        WHERE UserProgress.user_id = ? AND UserProgress.used_hint = 1 AND questions.lesson_id = ?
        """, (user_id, topic_id,))
    number_of_hints = len(cursor.fetchall())
    return number_of_hints

# Returns the percentage of questions seen for a topic
def topic_seen_percent(cursor, user_id, topic_id):
    cursor.execute("""
        SELECT COUNT(lesson_id)
        FROM questions
        WHERE lesson_id = ?
        """, (topic_id,))
    total_questions = cursor.fetchone()[0]
    if total_questions == 0:
        return 0
    cursor.execute("""
        SELECT COUNT(DISTINCT UserProgress.question_id)
        FROM UserProgress
        JOIN questions ON UserProgress.question_id = questions.question_id
        WHERE UserProgress.user_id = ? AND questions.lesson_id = ?
        """, (user_id, topic_id,))
    seen_questions = cursor.fetchone()[0]
    percent = seen_questions / total_questions * 100
    return percent
# Returns the total number of questions that a user has gotten correct at least once
def overall_total_correct(cursor, user_id):
    cursor.execute("""
        SELECT COUNT(DISTINCT UserProgress.question_id)
        FROM UserProgress
        JOIN questions ON UserProgress.question_id = questions.question_id
        WHERE UserProgress.user_id = ? AND is_correct = 1
        """, (user_id,))
    total_correct = cursor.fetchone()[0]
    return total_correct

# Returns the total number of questions
def overall_total_questions(cursor):
    cursor.execute("""
        SELECT COUNT(question_id)
        FROM questions
        """)
    total_questions = cursor.fetchone()[0]
    return total_questions

# Returns the percentage of questions the user has gotten correct at least once
def overall_percent(cursor, user_id):
    total_questions = overall_total_questions(cursor)
    if total_questions == 0:
        return 0
    total_correct = overall_total_correct(cursor, user_id)
    print(total_correct, total_questions)
    percent = total_correct / total_questions
    return percent

# Returns the total number of questions gotten correct at least once for a topic
def topic_total_correct(cursor, user_id, topic_id):
    cursor.execute("""
        SELECT COUNT(DISTINCT UserProgress.question_id)
        FROM UserProgress
        JOIN questions ON UserProgress.question_id = questions.question_id
        WHERE UserProgress.user_id = ? AND UserProgress.is_correct = 1 AND questions.lesson_id = ?
        """, (user_id, topic_id,))
    total_correct = cursor.fetchone()[0]
    return total_correct

# Returns the total numbers of questions in the topic
def topic_total_questions(cursor, topic_id):
    cursor.execute("""
        SELECT COUNT(lesson_id)
        FROM questions
        WHERE lesson_id = ?
        """, (topic_id,))
    total_questions = cursor.fetchone()[0]
    return total_questions

# Returns the total percentage of questions gotten correct at least once for a topic
def topic_correct_percent(cursor, user_id, topic_id):
    total_questions = topic_total_questions(cursor, topic_id)
    if total_questions == 0:
        return 0
    total_correct = topic_total_correct(cursor, topic_id)
    print(total_correct, total_questions)
    percent = total_correct / total_questions * 100
    return percent

# Returns the percentage of questions gotten correct for a mode
def mode_correct_percent(cursor, user_id, mode):
    cursor.execute("""
        SELECT COUNT(question_id)
        FROM UserProgress
        WHERE user_id = ? AND mode = ?
        """, (user_id, mode,))
    total_questions = cursor.fetchone()[0]
    if total_questions == 0:
        return 0
    cursor.execute("""
        SELECT COUNT(question_id)
        FROM UserProgress
        WHERE user_id = ? AND is_correct = 1 AND mode = ?
        """, (user_id, mode,))
    total_correct = cursor.fetchone()[0]
    percent = total_correct / total_questions * 100
    return percent

# Returns the percentage of questions gotten correct recently
def recent_correct_percent(cursor, user_id, history_length):
    if history_length == 0:
        return 0
    cursor.execute("""
        SELECT COUNT(progress_id)
        FROM (
            SELECT progress_id, is_correct
            FROM UserProgress
            WHERE user_id = ?
            ORDER BY progress_id DESC
            LIMIT ?
        ) AS recent
        WHERE is_correct = 1
        """, (user_id, history_length,))
    total_correct = cursor.fetchone()[0]
    percent = total_correct / history_length * 100
    return percent

# conn = sqlite3.connect('its_database.db')
# cursor = conn.cursor()
# print(topic_correct_percent(cursor, 1, 1))
